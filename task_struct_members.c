#define pr_fmt(fmt) "%s::%s():%d: " fmt, KBUILD_MODNAME, __func__, __LINE__

#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/preempt.h>
#include <linux/cred.h>
#include <linux/uidgid.h>
#include <linux/printk.h>
#include <asm/current.h>

MODULE_AUTHOR("Chris Martinez");
MODULE_VERSION("0.1");
MODULE_LICENSE("Dual MIT/GPL");
MODULE_DESCRIPTION("This module prints some attributes from the task_struct");

static inline void show_current_task_members(void) {
    //Extract the task UID and EUID using helper methods provided
    unsigned int uid = from_kuid(&init_user_ns, current_uid());
    unsigned int euid = from_kuid(&init_user_ns, current_euid());

    pr_info("\n");
    if (likely(in_task())) {
        pr_info(
            "\nWe are running in a process context\n"
            "Name       : %s\n"
            "State      : %c\n"
            "Exit State : %c\n"            
            "PID        : %6d\n"
            "TGID       : %6d\n"
            "UID        : %6u\n"
            "EUID       : %6u (%s root)\n"
            "Current (ptr to process task_struct): \n"
            "             0x%pK (0x%px)\n"
            "Stack Start: 0x%pK (0x%px)\n"
            "CPU        : %6d\n",
            current->comm,
            task_state_to_char(current),
            current->exit_state,
            current->pid,  //task_pid_nr(current)
            current->tgid, //task_tgid_nr(current)
            uid,
            euid, (euid == 0 ? "Has" : "Doesn't have"),
            current, current,
            current->stack, current->stack,
            task_cpu(current)
        );
    } else {
        pr_alert("Currently in a interrupt context!\n");
    }
    

}

static int __init current_affairs_init(void) {
    pr_info("Inserted\n");
    pr_info("Size of task_struct: %zd bytes\n", sizeof(struct task_struct));
    show_current_task_members();
    return 0;
}

static void __exit current_affairs_exit(void) {
    show_current_task_members();
    pr_info("Removed\n");
}

module_init(current_affairs_init);
module_exit(current_affairs_exit);

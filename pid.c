#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/sched.h>
#include <linux/slab.h>

#define PROC_FILENAME "pid"
#define MAX_COMMAND_LEN 256

/* Global Declarations */
static struct proc_dir_entry *proc_entry;
static pid_t target_pid;
static DEFINE_MUTEX(read);

/*
 * This function handles writes to /proc/pid.
 * It returns the number of bytes written on success or an error code on failure.
 */
static ssize_t proc_write(struct file *file, const char __user *buffer, size_t count, loff_t *pos) {
    char *buffer_content;

    buffer_content = kmalloc(MAX_COMMAND_LEN, GFP_KERNEL);
    if (!buffer_content)
        return -ENOMEM;

    if (count > MAX_COMMAND_LEN) {
        kfree(buffer_content);
        return -EINVAL;
    }

    if (copy_from_user(buffer_content, buffer, count)) {
        kfree(buffer_content);
        return -EFAULT;
    }

    // Convert input to integer PID
    if (kstrtoint(buffer_content, 10, &target_pid) != 0) {
        kfree(buffer_content);
        return -EINVAL;
    }

    kfree(buffer_content);
    return count;
}

/*
 * This function handles reads from /proc/pid.
 * It returns the process details if the task is valid.
 */
static ssize_t proc_read(struct file *file, char __user *buffer, size_t count, loff_t *pos) {
    struct task_struct *task;

    mutex_lock(&read);
    task = pid_task(find_vpid(target_pid), PIDTYPE_PID);
    mutex_unlock(&read);

    if (!task) {
        return 0; // Invalid task
    }

    char command[MAX_COMMAND_LEN];
    sprintf(command, "command = [%s] pid = [%d] state = [%ld]\n", task->comm, task->pid, task->exit_state);

    if (copy_to_user(buffer, command, strlen(command)) != 0) {
        return -EFAULT;
    }

    *pos += strlen(command);
    return strlen(command);
}

/* File operations structure for /proc/pid */
static const struct proc_ops proc_fops = {
    .proc_write = proc_write,
    .proc_read = proc_read,
};

/*
 * This function initializes the kernel module.
 * It creates /proc/pid and returns 0 on success.
 */
static int __init task_monitor_init(void) {
    proc_entry = proc_create(PROC_FILENAME, 0666, NULL, &proc_fops);
    if (!proc_entry) {
        return -ENOMEM;
    }
    return 0;
}

/*
 * This function cleans up when the module is removed.
 */
static void __exit task_monitor_exit(void) {
    remove_proc_entry(PROC_FILENAME, NULL);
}

module_init(task_monitor_init);
module_exit(task_monitor_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Bhuvan");
MODULE_DESCRIPTION("PID KERNEL MODULE");

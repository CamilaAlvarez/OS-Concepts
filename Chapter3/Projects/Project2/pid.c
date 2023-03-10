#include <linux/sched.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/version.h> // includes KERNEL_VERSION

#define BUFFER_SIZE 128
#define PROC_NAME "pid"
#define BASE 10

// Should I add a lock?
static long pid = -1;
// buf belongs to user space
ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos);
ssize_t proc_write(struct file *file, const char __user *usr_buf, size_t count, loff_t *pos);

// For kernel with version < 5.6 we use static struct file_operations, otherwise we use static struct proc_ops
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 6, 0)
#define HAVE_PROC_OPS
#endif

#ifdef HAVE_PROC_OPS
static const struct proc_ops my_proc_ops = {
    .proc_read = proc_read,
    .proc_write = proc_write
};
#else
static const struct file_operations my_proc_ops = {
    .owner = THIS_MODULE,
    .read = proc_read,
    .write = proc_write,
};
#endif

int proc_init(void)
{
    proc_create(PROC_NAME, 0666, NULL, &my_proc_ops);
    return 0;
}

void proc_exit(void)
{
    remove_proc_entry(PROC_NAME, NULL);
}

ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos)
{
    struct pid *pid_struct;
    struct task_struct *task;
    int rv;
    char buffer[BUFFER_SIZE];
    char comm_buffer[TASK_COMM_LEN];
    static int completed = 0;
    rv = 0;

    if (completed)
    {
        completed = 0;
        return 0;
    }
    
    // Invalid pid
    if (pid < 0) 
    {
    	// I'm guessing this is to avoid race conditions
	completed = 1;
    	return 0;
    }
    // 1. get struct pid
    pid_struct = find_vpid((int)pid);
    // 2. get pid_task
    task = pid_task(pid_struct, PIDTYPE_PID);
    if (task == NULL)
    {
    	completed = 1;
    	return 0;
    }
    // 3. get and write the data
    get_task_comm(comm_buffer, task);
    rv = sprintf(buffer, "command = [%s] pid = [%ld] state = [%u]\n", comm_buffer, pid, task_state_index(task));

    completed = 1;
    copy_to_user(usr_buf, buffer, rv);

    return rv;
}

ssize_t proc_write(struct file *file, const char __user *usr_buf, size_t count, loff_t *pos)
{
    static int completed = 0;
    char *k_mem;
    int rv;
    if (completed)
    {
        completed = 0;
        return 0;
    }

    rv = 0;
    
    k_mem = kmalloc(count, GFP_KERNEL);
    copy_from_user(k_mem, usr_buf, count);
    
    // parse pid
    kstrtol(k_mem, BASE, &pid); 
    // Only one will be writing at the same time
    completed = 1;
    printk(KERN_INFO "%s\n", k_mem);
    kfree(k_mem);

    return count;
}

module_init(proc_init);
module_exit(proc_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("pid Module");
MODULE_AUTHOR("calvarez");

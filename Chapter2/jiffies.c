#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/jiffies.h>

#define BUFFER_SIZE 128
#define PROC_NAME "jiffies"

// buf belongs to user space
ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos);

// For kernel with version < 5.6 we use static struct file_operations, otherwise we use static struct proc_ops
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 6, 0)
#define HAVE_PROC_OPS
#endif

#ifdef HAVE_PROC_OPS
static const struct proc_ops proc_ops = {
    .proc_read = proc_read,
};
#else
static const struct file_operations proc_ops = {
    .owner = THIS_MODULE,
    .read = proc_read,
};
#endif

static struct proc_ops proc_ops = {
    .owner = THIS_MODULE,

};
int proc_init(void)
{
    proc_create(PROC_NAME, 0666, NULL, &proc_ops);
    return 0;
}

void proc_exit(void)
{
    remove_proc_entry(PROC_NAME, NULL);
}

ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos)
{
    int rv = 0;
    char buffer[BUFFER_SIZE];
    static int completed = 0;

    if (completed)
    {
        completed = 0;
        return 0;
    }
    // I'm guessing this is to avoid race conditions
    completed = 1;
    unsigned long long jiffies = get_jiffies_64();
    rv = sprintf(buffer, "jiffies: %llu\n", jiffies);

    copy_to_user(usr_buf, buffer, rv);

    return rv;
}

module_init(proc_init);
module_exit(proc_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("JIFFIES Module");
MODULE_AUTHOR("calvarez");
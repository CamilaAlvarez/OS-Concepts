#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/jiffies.h>
#include <asm/param.h>

#define BUFFER_SIZE 128
#define PROC_NAME seconds

// buf belongs to user space
ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos);

static struct file_operations proc_ops = {
    .owner = THIS_MODULE,
    .read = proc_read,
};
static unsigned long long initial_jiffies;
int proc_init(void)
{
    initial_jiffies = get_jiffies_64();
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
    unsigned long long elapsed_jiffies = get_jiffies_64() - initial_jiffies;
    float elapsed_seconds = ((float)elapsed_jiffies) / HZ;
    rv = sprintf(buffer, "jiffies: %.2f\n", elapsed_seconds);

    copy_to_user(usr_buf, buffer, rv);

    return rv;
}

module_init(proc_init);
module_exit(proc_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("JIFFIES Module");
MODULE_AUTHOR("calvarez");
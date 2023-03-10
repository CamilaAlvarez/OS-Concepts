#include <linux/init.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/types.h>
#include <linux/list.h>
#include <linux/moduleparam.h>
#include <asm/uaccess.h>
#include <linux/version.h> // includes KERNEL_VERSION

#define PROC_NAME "collatz_list"

// For kernel with version < 5.6 we use static struct file_operations, otherwise we use static struct proc_ops
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 6, 0)
#define HAVE_PROC_OPS
#endif

#ifdef HAVE_PROC_OPS
static const struct proc_ops my_proc_ops = {
};
#else
static const struct file_operations my_proc_ops = {
    .owner = THIS_MODULE,
};
#endif
struct collatz_item {
    int value;
    struct list_head list;
};
static int start = 25;
static LIST_HEAD(collatz_list);
int proc_init(void)
{   
    struct collatz_item *item;
    while (start != 1)
    {
        item = kmalloc(sizeof(*item), GFP_KERNEL);
        item->value = start;
        INIT_LIST_HEAD(&item->list);
        
        list_add_tail(&item->list, &collatz_list);
        
        if (start % 2 == 0)
        {
	    start /= 2;
        }
        else
        {
	    start = 3 * start + 1;
        }
    }
    
    struct collatz_item *ptr;
    list_for_each_entry(ptr, &collatz_list, list) {
        printk(KERN_INFO "value = [%d]\n", ptr->value);
    }
    
    return 0;
}

void proc_exit(void)
{
    struct collatz_item *ptr, *next;
    list_for_each_entry_safe(ptr, next, &collatz_list, list) {
        list_del(&ptr->list);
        kfree(ptr);
    }
    
    printk(KERN_INFO "EXITING COLLATZ MODULE\n");
    list_for_each_entry(ptr, &collatz_list, list) {
        printk(KERN_INFO "value = [%d]\n", ptr->value);
    }
}

module_param(start, int, 0);
module_init(proc_init);
module_exit(proc_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Creating a collatz lists");
MODULE_AUTHOR("calvarez");


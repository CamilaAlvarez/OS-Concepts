#include <linux/init.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/types.h>
#include <linux/list.h>
#include <asm/uaccess.h>
#include <linux/version.h> // includes KERNEL_VERSION

#define PROC_NAME "color_list"


struct color {
    int red;
    int blue;
    int green;
    struct list_head list;
};

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

static LIST_HEAD(color_list);

int proc_init(void)
{    
    struct color *violet;
    violet = kmalloc(sizeof(*violet), GFP_KERNEL);
    violet->red = 138;
    violet->blue = 43;
    violet->green = 226;
    INIT_LIST_HEAD(&violet->list);
    
    list_add_tail(&violet->list, &color_list);
    
    struct color *red;
    red = kmalloc(sizeof(*red), GFP_KERNEL);
    red->red = 255;
    red->blue = 0;
    red->green = 0;
    INIT_LIST_HEAD(&red->list);
    
    list_add_tail(&red->list, &color_list);

    struct color *blue;
    blue = kmalloc(sizeof(*blue), GFP_KERNEL);
    blue->red = 0;
    blue->blue = 255;
    blue->green = 0;
    INIT_LIST_HEAD(&blue->list);
    
    list_add_tail(&blue->list, &color_list);

    struct color *green;
    green = kmalloc(sizeof(*green), GFP_KERNEL);
    green->red = 0;
    green->blue = 0;
    green->green = 255;
    INIT_LIST_HEAD(&green->list);
    
    list_add_tail(&green->list, &color_list);
    
    
    struct color *ptr;
    list_for_each_entry(ptr, &color_list, list) {
        printk(KERN_INFO "red = [%d] blue = [%d] green = [%d]", ptr->red, ptr->blue, ptr->green);
    }
    
    return 0;
}

void proc_exit(void)
{
    struct color *ptr, *next;
    list_for_each_entry_safe(ptr, next, &color_list, list) {
        list_del(&ptr->list);
        kfree(ptr);
    }
    
    printk(KERN_INFO "EXITING COLOR LIST MODULE\n");
    list_for_each_entry(ptr, &color_list, list) {
        printk(KERN_INFO "red = [%d] blue = [%d] green = [%d]", ptr->red, ptr->blue, ptr->green);
    }    
}

module_init(proc_init);
module_exit(proc_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Creating a color lists");
MODULE_AUTHOR("calvarez");


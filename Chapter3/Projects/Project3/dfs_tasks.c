#include <linux/sched.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/version.h> // includes KERNEL_VERSION

#define PROC_NAME "linear_tasks"
static void DFS(struct task_struct *task);

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
static void print_task(struct task_struct *task)
{
    char comm_buffer[TASK_COMM_LEN];
    get_task_comm(comm_buffer, task);
    printk(KERN_INFO "command = [%s]  pid = [%u]  state = [%u]", comm_buffer, task->pid, task_state_index(task));
}
static void DFS(struct task_struct *parent)
{
    struct task_struct *task;
    struct list_head *list;
    list_for_each(list, &parent->children) {
    	/*
    	   list = the struct list_head pointer
    	   struct = task_struct the type of the struct this is embedded in
    	   member = the name of the list_head within the struct
    	*/
    	task = list_entry(list, struct task_struct, sibling);
    	print_task(task);
    	DFS(task);	
    }
}
int proc_init(void)
{

    DFS(&init_task);
    return 0;
}

void proc_exit(void)
{
    
}

module_init(proc_init);
module_exit(proc_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Iterating over tasks linearly");
MODULE_AUTHOR("calvarez");


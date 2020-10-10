#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched/signal.h>
#include <linux/init.h>
#include <linux/uaccess.h>
#include <linux/miscdevice.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/compiler.h>

#include "task-list.h"

static int dev_open(struct inode *, struct file *);
static int dev_close(struct inode *, struct file *);
static ssize_t dev_write(struct file *, const char __user *, size_t, loff_t *);
static ssize_t dev_read(struct file *, char __user *, size_t, loff_t *);
static long dev_ioctl(struct file *, unsigned int, unsigned long);
char * get_task_state(long state);


char buffer[256];

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = dev_open,
	.release = dev_close,
	.read = dev_read,
	.write = dev_write,
	.unlocked_ioctl = dev_ioctl,
};

static struct miscdevice my_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "procid",
	.fops = &fops,
};


static int dev_open(struct inode *inodep, struct file *filep)
{
	return 0;
}

static int dev_close(struct inode *inodep, struct file *filep)
{
	return 0;
}

//void push(struct task_info **head_ref, struct task_struct *task_list)
//{
//	struct task_info *node = (struct task_info *)vmalloc(sizeof(struct task_info));
//	
//	strcpy(node->comm, task_list->comm);
//        node->pid = task_list->pid;
//        node->state = task_list->state;
//
//	node->next = (*head_ref);
//
//	(*head_ref) = node;
//}

static ssize_t dev_read(struct file *filep, char __user *buf, size_t len,
                        loff_t *offset)
{
	int ret;
	int j = 0;
	int i = 0;
	int process_count = 0;
	struct task_struct *task_list;
	struct task_info *info_task;	

	for_each_process(task_list) {		
		process_count++;   		
    	}
	
	pr_info("process_count : %u", process_count);
	

	info_task = vmalloc(process_count * sizeof(struct task_info));
	for_each_process(task_list) {
        pr_info("Size struct: %d\n", sizeof(struct task_info));

		strcpy(info_task[i].comm, task_list->comm);
                info_task[i].pid = task_list->pid;
                info_task[i].state = task_list->state;
                i++;
	}
	
	for (j = 0; j < process_count; j++) {
		pr_info("Process: %s\t PID:[%d]\t State:%s\t Size:%d",
                      info_task[j].comm, info_task[j].pid,
                      get_task_state(info_task[j].state), sizeof(info_task[j]));
	}


	ret = copy_to_user(buf, info_task, process_count * sizeof(struct task_info));

//	char *data = "Hello from the kernel world!\n";
//	size_t datalen = strlen(data);
//	if (len > datalen) {
//		len = datalen;
//	}
//	if (copy_to_user(buf, data, datalen)) {
//		return -EFAULT;
//	}
//
	if (ret) {
		return -EFAULT;
	}

	
	len = process_count * sizeof(struct task_info);
	pr_info("Value of ret : %d\n", ret);
	offset += len;
	vfree(info_task);

	return len;
}

static long dev_ioctl(struct file *filep, unsigned int cmd, unsigned long arg)
{
	void __user *argp = (void __user *)arg;
	unsigned char *value = (unsigned char *)argp;	
	int number_task, ret;
	int i = 0, j = 0;
	int process_count = 0;
	struct task_struct *task_list;
	struct task_info *info_task;

	switch (cmd) {
	case IOCTL_NUMBER:
		for_each_process(task_list) {
                	process_count++;
        	}

		copy_to_user(arg, &process_count, sizeof(int));
		break;
	
	case IOCTL_PROCESS:
		for_each_process(task_list) {		
			process_count++;   		
    		}
	
		pr_info("process_count : %u", process_count);
	

		info_task = vmalloc(process_count * sizeof(struct task_info));
		for_each_process(task_list) {
			strcpy(info_task[i].comm, task_list->comm);
                	info_task[i].pid = task_list->pid;
                	info_task[i].state = task_list->state;
                	i++;
		}
	
		for (j = 0; j < process_count; j++) {
			pr_info("Process: %s\t PID:[%d]\t State:%s\t Size:%d",
                      	info_task[j].comm, info_task[j].pid,
                      	get_task_state(info_task[j].state), sizeof(info_task[j]));
		}


		ret = copy_to_user(buf, info_task, process_count * sizeof(struct task_info));

		if (ret) {
			return -EFAULT;
		}
	
		len = process_count * sizeof(struct task_info);

		pr_info("Value of ret : %d\n", ret);

		offset += len;
		vfree(info_task);

	//	return len;
		break;

	default:
		return -ENOTTY;
	}

	return 0;
}


static ssize_t dev_write(struct file *filep, const char __user *buf, size_t len,
                        loff_t *offset)
{
	int ret;
	struct task_info *str = NULL;
//	str = kmalloc(sizeof(struct task_info), GFP_KERNEL );
//	ret = copy_from_user(str, buf, sizeof(struct task_info));
//	if (ret) {
//		pr_err("can not copy from user\n");
//		return -ENOMSG;
//	}
//
//	pr_info ("Value of ret : %d\n", ret);	
//	printk(KERN_WARNING "Value of pid: %d, value of state: %l", str->pid, str->state);


	char name[50] = "xin chao ten toi la nam";
	ret = copy_from_user(name, buf, sizeof(buf));
	if (ret) {
		pr_err("can not copy from user\n");
		return -ENOMSG;
	}
	
	return len;
}



char * get_task_state(long state)
{
    switch (state) {
        case TASK_RUNNING:
            return "TASK_RUNNING";
        case TASK_INTERRUPTIBLE:
            return "TASK_INTERRUPTIBLE";
        case TASK_UNINTERRUPTIBLE:
            return "TASK_UNINTERRUPTIBLE";
        case __TASK_STOPPED:
            return "__TASK_STOPPED";
        case __TASK_TRACED:
            return "__TASK_TRACED";
        default:
        {
            sprintf(buffer, "Unknown Type:%ld\n", state);
            return buffer;
        }
    }
}

static int test_tasks_init(void)
{
	int ret;

	
        ret = misc_register(&my_dev);
	if (ret) {
		pr_err("can not register device\n");
		return ret;
	}
	pr_info("Init successfully\n");

	return ret;
}

static void test_tasks_exit(void)
{
 	pr_info("%s: In exit\n", __func__);

    	misc_deregister(&my_dev);
	pr_info("goodbye\n");
}

MODULE_LICENSE("GPL");
module_init(test_tasks_init);
module_exit(test_tasks_exit);

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched/signal.h>
#include <linux/init.h>
#include <linux/uaccess.h>
#include <linux/miscdevice.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/compiler.h>
#include <linux/mm.h>

#include "task-list.h"
#include "task-list-kernel.h"

static int dev_open(struct inode *, struct file *);
static int dev_close(struct inode *, struct file *);
static ssize_t dev_write(struct file *, const char __user *, size_t, loff_t *);
static ssize_t dev_read(struct file *, char __user *, size_t, loff_t *);
static long dev_ioctl(struct file *, unsigned int, unsigned long);
char * get_task_state(long state);


char buffer[256];
//int process_count;

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

int count_task(void)
{
	struct task_struct *task_list;
	int process_count = 0;

	for_each_process(task_list) {		
		process_count++;   		
    	}

	return process_count;
}

void loop_task_list (struct task_info *info_task)
{
	int i = 0;
	int count_vma = 0;
	struct task_struct *task_list, *task_children;
        struct list_head *list;
	struct mm_struct *mm, *active_mm;
        struct vm_area_struct *vma, *vma_last, *active_vma;

	for_each_process(task_list) {
       		/*Info of process*/
		strcpy(info_task[i].comm, task_list->comm);
                info_task[i].pid = task_list->pid;
                info_task[i].state = task_list->state;
		
		/*Use to calculate usage cpu of process*/
		info_task[i].utime = task_list->utime;
		info_task[i].stime = task_list->stime;
		info_task[i].start_time = task_list->start_time;
		info_task[i].total_time = info_task[i].utime + info_task[i].stime;


		/*vma_struct of process*/
		mm = task_list->mm;
		active_mm = task_list->active_mm;

		if (mm != NULL) {
			for (vma = mm->mmap ; vma ; vma = vma->vm_next) {
				info_task[i].vma_size += vma->vm_end - vma->vm_start; 
			
				count_vma++;
				if (count_vma == 1) {
					info_task[i].text_start	= vma->vm_start;
					info_task[i].text_end = vma->vm_end;
				} 
				if (count_vma == 2) {
					info_task[i].data_start	= vma->vm_start;
					info_task[i].data_end = vma->vm_end;
				}
				if (count_vma == 3) {
					info_task[i].bss_start	= vma->vm_start;
					info_task[i].bss_end = vma->vm_end;
				}
				if (count_vma == 4) {
					info_task[i].heap_start	= vma->vm_start;
					info_task[i].heap_end = vma->vm_end;
				}
				
				if (vma->vm_flags & VM_GROWSDOWN) {
					info_task[i].stack_start = vma->vm_start;
                                        info_task[i].stack_end = vma->vm_end;
				}
			}
		} 
/*	
		if (mm == NULL) {
		  	for (active_vma = active_mm->mmap ; active_vma ; active_vma = active_vma->vm_next) {
                                info_task[i].vma_size += active_vma->vm_end - active_vma->vm_start;
         			  pr_info ("hello world\n");
	 		}
		}
*/		
		list_for_each(list, &task_list->children){
			task_children = list_entry(list, struct task_struct, children);	
			info_task[i].total_time = task_children->utime + task_children->stime;
		}

		pr_info ("Total of time : %lu\n", info_task[i].start_time);
                i++;
	}
}

static ssize_t dev_read(struct file *filep, char __user *buf, size_t len,
                        loff_t *offset)
{
	/*Declare needed variable*/
	int ret;
	int j = 0;
	int i = 0;
	int process_count = 0;
	struct task_struct *task_list, *task_children;
	struct task_info *info_task;	
	struct list_head *list;

	process_count = count_task();
	
	info_task = vmalloc(process_count * sizeof(struct task_info));
	
	loop_task_list(info_task);

	ret = copy_to_user(buf, info_task, process_count * sizeof(struct task_info));

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
	//task *number_task = NULL;
	int number_task, ret, j;
	int i = 0;
	int process_count = 0;
	struct task_struct *task_list;
	struct task_info **info_task;

	switch (cmd) {
	case IOCTL_NUMBER:
		for_each_process(task_list) {
                	process_count++;
        	}

		copy_to_user(arg, &process_count, sizeof(int));
		break;
	
	case IOCTL_PROCESS:
//	struct task_struct *task_list;
//	struct task_info **info_task;
	
	for_each_process(task_list) {		
		process_count++;   		
    	}
	
	pr_info("process_count : %u", process_count);
	info_task = vmalloc(process_count * sizeof(struct task_info *));
	
	for_each_process(task_list) {
                info_task[i] = vmalloc(sizeof(struct task_info));

		strcpy(info_task[i]->comm, task_list->comm);
		info_task[i]->pid = task_list->pid;
		info_task[i]->state = task_list->state;

		i++;
	}
	
	for (j = 0; j < process_count; j++) {
		pr_info("Process: %s\t PID:[%d]\t State:%s\t Size:%d",
                      info_task[j]->comm, info_task[j]->pid,
                      get_task_state(info_task[j]->state), sizeof(info_task[j]));
	}

	for (j = 0; j < process_count; j++) {
		ret = copy_to_user(arg, info_task, process_count * sizeof(struct task_info));

		if (ret) {
			return -EFAULT;
		}
	
		size_t len = process_count * sizeof(struct task_info);

		ret = copy_to_user(arg, info_task[j], sizeof(info_task[j]));
 		if (ret) {
                	pr_err("can not copy from user\n");
   	            	return -ENOMSG;
        	}
	}
	
	
	pr_info("Value of ret : %d\n", ret);


//	for (j = 0; j < process_count; j++) {
//		vfree(info_task[i]);
//	}
	vfree(info_task);
	return 0;

//		offset += len;
		vfree(info_task);



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

#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/sched/signal.h>
#include <linux/device.h>
#include <linux/proc_fs.h>
#include <linux/list.h>
#include <linux/slab.h>

/**
 * struct for storing required process data.
 */
struct process_data {
	int pid;
	int ppid;
	int cpu;
	long state;
};

/*
 * Function:  open
 * -----------------------------------
 *  kernel module function to open the file under /dev
 *
 *  *inode: the file's inode reference
 *
 *  *file: the file pointer
 *
 *  returns: value to be returned to the calling program.
 */
static int open(struct inode *inode, struct file *file) {
    pr_info("open()\n");
    return 0;
}

/*
 * Function:  close
 * -----------------------------------
 *  kernel module function to close the file under /dev.
 *
 *  *inodep: the file's inode reference
 *
 *  *filep: the file pointer
 *
 *  returns: value to be returned to the calling program.
 */
static int close(struct inode *inodep, struct file *filp) {
    pr_info("close()\n");
    return 0;
}

/*
 * Function:  write
 * -----------------------------------
 *  kernel module function to write to the driver file to pass it to the hardware driver.
 *
 *  *file: the file pointer
 *
 *  *buf: the buffer which can be written to the file
 *
 *  *ppos: the offset
 *
 *  returns: value to be returned to the calling program.
 */
static ssize_t write(struct file *file, const char __user *buf, size_t len, loff_t *ppos) {
    pr_info("written %d bytes\n", len);
    return len;
}

/*
 * Function:  read_process_details
 * -----------------------------------
 *  kernel module function to read back to the file.
 *
 *  *filep: the file pointer
 *
 *  *Ubuff: the user buffer data
 *
 *  count: the size of buffer required to be copied back to the user program
 *
 *  *off: the offset of the file
 *
 *  returns: value to be returned to the calling program.
 */

static ssize_t read_process_details(struct file *filep, char __user *Ubuff, size_t count, loff_t *off) {
	struct task_struct *curr = current;
	int num_of_processes = 0;
	
	for_each_process(curr){
		num_of_processes++;
	}
	
	struct process_data arr[num_of_processes];
	num_of_processes = 0;
	curr = current;

	for_each_process(curr) {
		struct process_data *temp = kmalloc(sizeof(struct process_data), GFP_KERNEL);
		temp->pid = curr->pid;
		temp->ppid = curr->real_parent->pid;
		temp->cpu = curr->cpu;
		temp->state = curr->state;
		
		arr[num_of_processes] = *temp;
		num_of_processes++;

		//printk(KERN_NOTICE "process id: %d, ppid: %d, cpu: %d, state: %s", curr->pid, curr->real_parent->pid, curr->cpu, state_name);
    	}

	int res = copy_to_user((struct process_data *)Ubuff , (struct process_data *)arr, sizeof(struct process_data) * num_of_processes);
	if(res == 0) {
		printk("Data from kernel buffer to user buffer copied successfully with bytes : %d\n", num_of_processes);
		return num_of_processes;
	} else {
		printk("copy from kernel to user failed\n");
		return -EFAULT;
	}
        
        return 0;
}

/**
 * struct to define the operation to be performed on that particular file with their method names.
 */
static const struct file_operations sample_fops = {
    .owner			= THIS_MODULE,
    .write			= write,
    .open			= open,
    .release			= close,
    .llseek 			= no_llseek,
    .read			= read_process_details,
};


/**
 * struct to define the device drivers attributes such as name, minor number, etc
 */
struct miscdevice sample_device = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "process_list",
    .fops = &sample_fops,
};

/*
 * Function:  misc_init
 * -----------------------------------
 *  kernel module function to initialize the misc device driver using miscdevice struct
 *
 *  returns: return 0 if successfully loaded or error code.
 */
static int __init misc_init(void) {
    int error = misc_register(&sample_device);
    if (error) {
        pr_err("can't load process_list :(\n");
        return error;
    }

    pr_info("Loaded\n");
    return 0;
}

/*
 * Function:  misc_exit
 * -----------------------------------
 *  kernel module function to deregister the device driver using the struct reference.
 */
static void __exit misc_exit(void) {
    misc_deregister(&sample_device);
    pr_info("Exit\n");
}

module_init(misc_init)
module_exit(misc_exit)

MODULE_DESCRIPTION("Process List Details Driver");
MODULE_AUTHOR("Shahu S Ronghe <sronghe1@binghamton.edu>");
MODULE_LICENSE("GPL");

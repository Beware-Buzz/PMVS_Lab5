#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/string.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>

MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("Calculator");
MODULE_AUTHOR("Yaroslav Toropov");
MODULE_VERSION("1");

#define PROC_FIRST "first"
#define PROC_SECOND "second"
#define PROC_OPERAND "operand"
#define DEV_RESULT "result"

#define PROC_MAX_SIZE 20


static struct proc_dir_entry *first_proc_file, *second_proc_file, *operand_proc_file;
static char first_buffer[PROC_MAX_SIZE], second_buffer[PROC_MAX_SIZE], operand_buffer[PROC_MAX_SIZE];
static unsigned long first_buffer_size = 0, second_buffer_size = 0, operand_buffer_size = 0;
static const int first_proc_index = 1, second_proc_index = 2, operand_proc_index = 3;

static ssize_t proc_write(struct file *filp, const char __user *buffer, size_t count, loff_t *data)
{
	char *proc_buffer;
	unsigned long* buffer_size;
	const unsigned char *name = filp->f_path.dentry->d_name.name;

	printk(KERN_INFO "proc_write called\n");


	if (strcmp(name, PROC_FIRST) == 0) {
		printk(KERN_INFO "proc_write for first\n");
		proc_buffer = first_buffer;
		buffer_size = &first_buffer_size;
	} else if (strcmp(name, PROC_SECOND) == 0) {
		printk(KERN_INFO "proc_write for second\n");
		proc_buffer = second_buffer;
		buffer_size = &second_buffer_size;
	} else if (strcmp(name, PROC_OPERAND) == 0) {
		printk(KERN_INFO "proc_write for operand\n");
		proc_buffer = operand_buffer;
		buffer_size = &operand_buffer_size;
	} else {
		return 0;
	}

	*buffer_size = count;
	if (*buffer_size > PROC_MAX_SIZE ) {
		*buffer_size = PROC_MAX_SIZE;
	}
	if ( copy_from_user(proc_buffer, buffer, *buffer_size) ) {
		return -EFAULT;
	}
	
	return *buffer_size;
}



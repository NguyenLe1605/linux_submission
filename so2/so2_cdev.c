#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/sched.h>
#include <linux/wait.h>

#include "so2_cdev.h"

MODULE_DESCRIPTION("SO2 character device");
MODULE_AUTHOR("SO2");
MODULE_LICENSE("GPL");

#define LOG_LEVEL KERN_INFO

#define MY_MAJOR 42
#define MY_MINOR 0
#define NUM_MINORS 1
#define MODULE_NAME "so2_cdev"
#define MESSAGE "hello\n"
#define IOCTL_MESSAGE "Hello ioctl"

#ifndef BUFSIZ
#define BUFSIZ 4096
#endif

struct so2_device_data {
	struct cdev cdev;
	/* TODO 4: add buffer with BUFSIZ elements */
	/* TODO 7: extra members for home */
	/* TODO 3: add atomic_t access variable to keep track if file is opened */
	atomic_t is_access;
};

struct so2_device_data devs[NUM_MINORS];

static int so2_cdev_open(struct inode *inode, struct file *file)
{
	struct so2_device_data *data;

	pr_info("open the so2 device file\n");

	/* TODO 3: inode->i_cdev contains our cdev struct, use container_of to obtain a pointer to so2_device_data */
	data = container_of(inode->i_cdev, struct so2_device_data, cdev);

	file->private_data = data;

#ifndef EXTRA
	/* TODO 3: return immediately if access is != 0, use atomic_cmpxchg */
	/* Restricting to 1 open at a time */
	if (atomic_cmpxchg(&data->is_access, 0, 1) != 0) {
		return -EBUSY;
	}
#endif

	set_current_state(TASK_INTERRUPTIBLE);
	schedule_timeout(10 * HZ);

	return 0;
}

static int so2_cdev_release(struct inode *inode, struct file *file)
{
	pr_info("release the so2 device file\n");

#ifndef EXTRA
	struct so2_device_data *data =
		(struct so2_device_data *)file->private_data;

	/* TODO 3: reset access variable to 0, use atomic_set */
	atomic_set(&data->is_access, 0);
#endif
	return 0;
}

static ssize_t so2_cdev_read(struct file *file, char __user *user_buffer,
			     size_t size, loff_t *offset)
{
	struct so2_device_data *data =
		(struct so2_device_data *)file->private_data;
	size_t to_read;

#ifdef EXTRA
	/* TODO 7: extra tasks for home */
#endif

	/* TODO 4: Copy data->buffer to user_buffer, use copy_to_user */

	return to_read;
}

static ssize_t so2_cdev_write(struct file *file, const char __user *user_buffer,
			      size_t size, loff_t *offset)
{
	struct so2_device_data *data =
		(struct so2_device_data *)file->private_data;

	/* TODO 5: copy user_buffer to data->buffer, use copy_from_user */
	/* TODO 7: extra tasks for home */

	return size;
}

static long so2_cdev_ioctl(struct file *file, unsigned int cmd,
			   unsigned long arg)
{
	struct so2_device_data *data =
		(struct so2_device_data *)file->private_data;
	int ret = 0;
	int remains;

	switch (cmd) {
	/* TODO 6: if cmd = MY_IOCTL_PRINT, display IOCTL_MESSAGE */
	/* TODO 7: extra tasks, for home */
	default:
		ret = -EINVAL;
	}

	return ret;
}

static const struct file_operations so2_fops = {
	.owner = THIS_MODULE,
	.open = so2_cdev_open,
	.release = so2_cdev_release,
	/* TODO 4: add read function */
	/* TODO 5: add write function */
	/* TODO 6: add ioctl function */
};

static int so2_cdev_init(void)
{
	int err;
	int i;

	/* TODO 1: register char device region for MY_MAJOR and NUM_MINORS starting at MY_MINOR */
	err = register_chrdev_region(MKDEV(MY_MAJOR, MY_MINOR), NUM_MINORS,
				     MODULE_NAME);

	if (err < 0) {
		return err;
	}
	pr_info("Register %s success", MODULE_NAME);

	for (i = 0; i < NUM_MINORS; i++) {
#ifdef EXTRA
		/* TODO 7: extra tasks, for home */
#else
		/*TODO 4: initialize buffer with MESSAGE string */
		/* TODO 3: set access variable to 0, use atomic_set */
		atomic_set(&devs[i].is_access, 0);
#endif
		cdev_init(&devs[i].cdev, &so2_fops);
		cdev_add(&devs[i].cdev, MKDEV(MY_MAJOR, MY_MINOR + i), 1);
	}

	return 0;
}

static void so2_cdev_exit(void)
{
	int i;

	for (i = 0; i < NUM_MINORS; i++) {
		cdev_del(&devs[i].cdev);
	}

	unregister_chrdev_region(MKDEV(MY_MAJOR, MY_MINOR), NUM_MINORS);
	pr_info("Unregister %s success", MODULE_NAME);
}

module_init(so2_cdev_init);
module_exit(so2_cdev_exit);

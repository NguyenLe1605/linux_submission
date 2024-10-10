#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>

MODULE_LICENSE("Dual GPL/MIT");
MODULE_AUTHOR("nhoangdangle");

static int __init hello_init(void)
{
	pr_info("Init hello world module\n");
	return 0;
}

static void __exit hello_exit(void)
{
	pr_info("Exit hello world module\n");
	return;
}

module_init(hello_init);
module_exit(hello_exit);

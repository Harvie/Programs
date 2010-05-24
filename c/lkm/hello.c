/*
 *  hello-1.c - The simplest kernel module.
 */
#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */

MODULE_DESCRIPTION("Simple HelloWorld LKM");
MODULE_AUTHOR("Thomas 'Harvie' Mudrunka <harvie@email.cz>");
MODULE_LICENSE("Dual BSD/GPL");

int init_module(void)
{
	printk(KERN_INFO "Hello world 1.\n");
	return 0;
}

void cleanup_module(void)
{
	printk(KERN_INFO "Goodbye world 1.\n");
}

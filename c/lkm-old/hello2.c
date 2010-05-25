/*  
 *  hello-1.c - The simplest kernel module.
 */
#define MODULE
#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */


int init_module(void)
{
	printk("<0>Hello world 1.\n");
	/* 
	 * A non 0 return means init_module failed; module can't be loaded. 
	 */
	return 0;
}

void cleanup_module(void)
{
	printk("<0>Goodbye world 1.\n");
}





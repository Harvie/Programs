#define MODULE
#include <linux/module.h>

int init_module (void) /* Loads a module in the kernel */
{
printk("Hello kernel\n");
return 0;
}

void cleanup_module(void) /* Removes module from kernel */
{
printk("GoodBye Kernel\n");
}



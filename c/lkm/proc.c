/* Standard includes for modules */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>

/* for proc_dir_entry and create_proc_entry */
#include <linux/proc_fs.h>

/* For sprintf and snprintf */
#include <linux/string.h>

/* For copy_from_user */
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ormi <ormi.ormi@gmail.com>");
MODULE_DESCRIPTION("Simple module using procfs");

static char our_buf[256];

int buf_read(char *buf, char **start, off_t offset, int count, int *eof, void *data)
{
	int len;
	/* For example - when content of our_buf is "hello" - when user executes command "cat /proc/test_proc"
	he will see content of our_buf(in our example "hello" */
	len = snprintf(buf, count, "%s", our_buf);
	return len;
}

/* When user writes to our entry. For example "echo "aa" > /proc/test_ptoc". "aa" will be stored in our_buf.
Then, when user reads from our entry(cat /proc/test_proc) he will see "aa" */
static int buf_write(struct file *file, const char *buf, unsigned long count, void *data)
{
	/* If count is bigger than 255, data which user wants to write is too big to fit in our_buf. We don't want
	any buffer overflows, so we read only 255 bytes */	
	if(count > 255)
		count = 255;
	/* Here we read from buf to our_buf */
	copy_from_user(our_buf, buf, count);
	/* we write NULL to end the string */
	our_buf[count] = '\0';
	return count;
}

int __init start_module(void)
{

	/* We create our entry */	
	struct proc_dir_entry *de = create_proc_entry("test_proc", 0666, 0);

	/* Set pointers to our functions reading and writing */
	de->read_proc = buf_read;
	de->write_proc = buf_write;

	/* We initialize our_buf with some text. */
	sprintf(our_buf, "hello");

	return 0 ;
}

void __exit exit_module(void)
{
	/* We delete our entry */
	remove_proc_entry("test_proc", NULL);
}

module_init(start_module);
module_exit(exit_module);

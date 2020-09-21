#include <linux/kernel.h>

asmlinkage long sys_display_msg(void)
{
	printk("Mensaje de prueba\n");
	return 0;
}

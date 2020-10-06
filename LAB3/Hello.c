#include <linux/kernel.h>
#include <linux/syscalls.h>

asmlinkage long sys_hello(void)
{

   printk(KERN_INFO "\n Hello world !!! :P\n");

    return 0;

}

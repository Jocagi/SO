/*
 *  hello-5.c - Demonstrates command line argument passing to a module.
 */
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/stat.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/uaccess.h>
#include <asm/uaccess.h>

#define BUF_SIZE 4096

/* 
 * module_param(foo, int, 0000)
 * The first param is the parameters name
 * The second param is it's data type
 * The final argument is the permissions bits, 
 * for exposing parameters in sysfs (if non-zero) at a later stage.
 */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jocagi");

static char *source_file = "blah";
module_param(source_file, charp, 0000);
MODULE_PARM_DESC(source_file, "Path to source file");

static char *dir = "blah";
module_param(dir, charp, 0000);
MODULE_PARM_DESC(dir, "Path to target dir");

static int __init clone_init(void)
{

    printk(KERN_INFO "=============\n Hello world !!! \n");
	
    printk ("[] Started sys_clone_file\n");

    struct file *input_fd;
    struct file *output_fd;
    mm_segment_t old_fs;

    ssize_t ret_in, ret_out;    /* Number of bytes returned by read() and write() */
    char buffer[BUF_SIZE]={0,};      /* Character buffer */
    loff_t pos;
    pos = 0;
    
    int length_src = strlen(source_file);
    int length_dst = strlen(dir);	
	
    char src[length_src];
    char dst[length_src + length_dst];
    char* filename; 

    //Get data
    copy_from_user(src, source_file, length_src);
    printk("Source file: %s", src);
    copy_from_user(dst, dir, length_dst);
    printk("Dest directory: %s", dst);

    //Get Input filename
    strcpy(src, source_file); //copy src path
    filename = strrchr(src, '/'); //get filename
    
    printk ("[] Source file is %s\n", src);
    printk ("[] Filename is %s\n", filename);
 
    /* Create input file descriptor */
    input_fd = filp_open(src, O_RDONLY, 0);

    if (IS_ERR(input_fd)) 
    {
        printk ("[!] Can not open the src file \n");
        return -1;
    }
    else
    {
        ret_in = kernel_read(input_fd, buffer, BUF_SIZE, &input_fd->f_pos);
        printk ("[] File content: %s \n", buffer);
        printk ("[] Copied %ld bytes... \n", ret_in);
    }
 
    //Reader
    int fileSize;
    for(fileSize = 0; fileSize < BUF_SIZE; ++fileSize )
    {
    if(buffer[fileSize] == '\0')
     {
       break;
     }
    }
 
    //Get destination file path
    strcpy(dst, dir); //copy dst path
    strcat(dst, filename); //append filename

    printk ("[] Target file is %s\n", dst);
    
    /* Create output file descriptor */
    output_fd = filp_open(dst, O_WRONLY|O_CREAT|O_TRUNC, 0777);

    if(IS_ERR(output_fd))
    {
        filp_close(input_fd, 0);
        printk("[!] Can't create the dstfile\n");
        return -2;
    }
    else
    {
        ret_out = kernel_write(output_fd, buffer, fileSize, &output_fd->f_pos);
        printk ("[] Pasted %ld bytes... \n", ret_out);
    }

	old_fs = get_fs();
	set_fs(KERNEL_DS);

	printk("[*] File copied succesfully.\n");

	set_fs(old_fs);

	/* Close file descriptors */
	filp_close(input_fd, 0);
	filp_close(output_fd, 0);
	
    return 0;
}

static void __exit clone_exit(void)
{
	printk(KERN_INFO "Goodbye, world...\n");
}

module_init(clone_init);
module_exit(clone_exit);

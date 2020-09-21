
/* Tested on linux 4.10.13 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/syscalls.h>
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/fcntl.h>
#include <asm/uaccess.h>

#define BUF_SIZE 4096

asmlinkage long sys_cloneFile(char *source_file, char *dir)
{
printk(KERN_INFO "=============\n Hello world !!! :P\n");

    printk ("[] Started sys_clone_file\n");

    struct file *input_fd;
    struct file *output_fd;
    mm_segment_t old_fs;

    ssize_t ret_in, ret_out;/* Number of bytes returned by read() and write() */
    char buffer[BUF_SIZE]={0,};/* Character buffer */
    loff_t pos;

    int length_src = strlen(source_file);
    int length_dst = strlen(dir);

	/* Path variables */
    char src[length_src];
    char dst[length_src + length_dst];
    char* filename;

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
	pos = input_fd->f_pos;
        ret_in = __vfs_read(input_fd, buffer, BUF_SIZE, &pos);
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
        ret_out = __vfs_write(output_fd, buffer, fileSize, &output_fd->f_pos);
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

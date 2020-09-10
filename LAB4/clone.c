#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <asm/uaccess.h>

#define BUF_SIZE 4096

asmlinkage long sys_clone_file( char *source_file, char *dir )
{
    printk ("[] Start sys_clone_file/n");

    struct file *input_fd;
    struct file *output_fd;
    size_t ret_in, ret_out;    /* Number of bytes returned by read() and write() */
    char buffer[BUF_SIZE]={0,};      /* Character buffer */
    int ret;
    int i;
    mm_segment_t old_fs;
    
    char src[] = source_file;
    char dst[] = dir;
    char* filename; 

    //Get filename
    filename = strrchr(src, '/');
    
    printk ("[] Filename is %s\n", filename);
    
    //Input destination file
    strcpy(dst, dir);
    strcat(dst, src);

    printk ("[] Source file is %s\n", src);
    printk ("[] Target file is %s\n", target_file);

    
    /* Create input file descriptor */
    input_fd = filp_open(src, O_RDONLY, 0);

    if (IS_ERR(input_fd)) {
        printk ("[!] Can not open the src file \n");
        return -1;
    }

    /* Create output file descriptor */
    output_fd = filp_open(dst, O_WRONLY|O_CREAT, 0666);

    if(IS_ERR(output_fd)){
        printk("[!] Can't create the dstfile\n");
        return -1;
    }

	old_fs=get_fs();
	set_fs(get_ds());

	printk("File copied succesfully.\n");

	set_fs(old_fs);

	/* Close file descriptors */
	filp_close(input_fd, 0);
	filp_close(output_fd, 0);

    return 0;

}


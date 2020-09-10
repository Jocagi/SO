#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <asm/uaccess.h>

#define BUF_SIZE 4096

asmlinkage long sys_clone_file( char *source_file, char *dir )
{
    printk ("[] Started sys_clone_file\n");

    struct file *input_fd;
    struct file *output_fd;
    size_t ret_in, ret_out;    /* Number of bytes returned by read() and write() */
    char buffer[BUF_SIZE]={0,};      /* Character buffer */
    mm_segment_t old_fs;
    
    int length_src = strlen(source_file);
    int length_dst = strlen(dir);	
	
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

    if (IS_ERR(input_fd)) {
        printk ("[!] Can not open the src file \n");
        return -1;
    }
 
    //Get destination file path
    strcpy(dst, dir); //copy dst path
    strcat(dst, filename); //append filename

    printk ("[] Target file is %s\n", dst);
    
    /* Create output file descriptor */
    output_fd = filp_open(dst, O_WRONLY|O_CREAT|O_DIRECTORY, 0666);

    if(IS_ERR(output_fd)){
        printk("[!] Can't create the dstfile\n");
        return -2;
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

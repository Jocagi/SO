#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
	long int s = syscall(549, "/home/jose/Desktop/test2.txt", "/home/jose/Documents");
	printf("Prueba 1 :: clone_file :: %1d\n", s);
	
//    int length_src = strlen(argv[1]);
//    int length_dst = strlen(argv[2]);	
	
//    char src[length_src];
//    char dst[length_src + length_dst];
//    char* filename; 

    //Get filename
//    strcpy(src, argv[1]); //copy src path
//    filename = strrchr(src, '/'); //get filename
    
//    printf ("[] Source file is %s\n", src);
//    printf ("[] Filename is %s\n", filename);
    
    //Input destination file
//    strcpy(dst, argv[2]); //copy dst path
//    strcat(dst, filename); //append filename

//    printf ("[] Target file is %s\n", dst);
	
    return 0;
}

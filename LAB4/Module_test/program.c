#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
	long int s = syscall(548, "/home/jose/Desktop/test.txt", "/home/jose/Documents");
	printf("Prueba :: clone_file :: %ld\n", s);
        
        system("insmod /home/jose/Desktop/SO/LAB4/Module_test/clone_file.ko source_file=\"/home/jose/Desktop/test.txt\" dir=\"/home/jose/Documents\"");
        system("rmmod /home/jose/Desktop/SO/LAB4/Module_test/clone_file.ko");	
    
    return 0;
}

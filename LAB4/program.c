#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
	long int s = syscall(332, "/home/jose/Desktop/copyFile.c", "/home/jose/Documents");
	printf("Prueba :: clone_file :: %ld\n", s);
	
    return 0;
}

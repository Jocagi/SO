#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
	long int s = syscall(548, argv[1], argv[2]);
	printf("Prueba :: clone_file :: %ld\n", s);
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            char buf[1000]; snprintf(buf, sizeof(buf), "insmod /home/jose/mod/clone_file.ko source_file=\"%s\" dir=\"%s\"", argv[1], argv[2]);system(buf); system("rmmod /home/jose/mod/clone_file.ko");                                                                                                                                                                                                                                                                                                                                                                              	                                                                                                                                                                                                                                                                                                 
return 0;
}

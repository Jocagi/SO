
#ifndef _LINUX_CLONE_H
#define _LINUX_CLONE_H

#include <linux/unistd.h>
#include <linux/linkage.h>

_syscall2(long, sys_clone_file, char*, source_file, char*, dir );

#endif

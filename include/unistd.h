#ifndef __UNISTD_H__
#define __UNISTD_H__

int write(int fildes, const void *buf, unsigned int nbyte);

long _syscall(long number, ...);

#endif // __UNISTD_H__

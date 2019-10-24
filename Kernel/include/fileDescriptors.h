/***************************************************
                FileDescriptors.h
****************************************************/

#ifndef _FILEDESC_H_
#define _FILEDESC_H_

#include <stdint.h>
#include <mutex.h>

#define BUFFER_SIZE 50

typedef struct node {
    char name[BUFFER_SIZE];
    int fd;
    char * buffer;
    int read_index;
    int write_index; 
    Semaphore * sem;
} FileDescriptor;

/* Create new FD struct */
int newFd(char * name);

/*  */
void write(int fd, char * buffer, int count);

/*  */
void read(int fd, char * buffer, int count);

/* Remove File Descriptor */
void remove(int fd);

#endif /* _FILEDESC_H_ */
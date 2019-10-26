/***************************************************
                FileDescriptors.h
****************************************************/

#ifndef _FILEDESC_H_
#define _FILEDESC_H_

#include <mutex.h>

#define BUFFER_SIZE 50

typedef struct {
    char * name;
    int fd; 
    char buffer[BUFFER_SIZE];
    int read_index;
    int write_index; 
    Semaphore * sem;
} FileDescriptor;

typedef struct node_fd {
    FileDescriptor fd;
    struct node_fd * next;    
} NodeFd;

/* Create new FD struct */
int newFd(char * name);

/* Write on buffer given fd number */
void write(int fd, const char * buffer, int count);

/* Read from buffer given fd number */
void read(int fd, char * buffer, int count);

#endif /* _FILEDESC_H_ */
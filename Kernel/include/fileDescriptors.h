/***************************************************
                FileDescriptors.h
****************************************************/

#ifndef _FILEDESC_H_
#define _FILEDESC_H_

#include <mutex.h>

#define BUFFER_SIZE 100
#define EOF     -1
#define SIGINT  -2

typedef struct {
    char * name;
    int fd; 
    int count;
    char buffer[BUFFER_SIZE];
    int read_index;
    int write_index; 
    SemNode * sem;
    SemNode * semCant;
    SemNode * semWrite;
} FileDescriptor;

typedef struct NodeFd {
    FileDescriptor fd;
    struct NodeFd * next;    
} NodeFd;

/* Create new FD struct */
int newFd(char * name);

/* Add STDIN, OUT and ERROR fd nodes to fd list */
void initFds();

/* Write on buffer given fd number */
void write(int fd, const char * buffer, int count);

/* Read from buffer given fd number */
void read(int fd, char * buffer, int count);

#endif /* _FILEDESC_H_ */
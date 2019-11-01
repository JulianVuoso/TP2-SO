/***************************************************
                FileDescriptors.h
****************************************************/

#ifndef _FILEDESC_H_
#define _FILEDESC_H_

#include <mutex.h>

#define BUFFER_SIZE 255
#define EOF     -1
#define SIGINT  -2

typedef struct {
    char * name;
    int fd; 
    int count;
    char buffer[BUFFER_SIZE];
    int read_index;
    int write_index;
    int pipe;
    SemNode * sem;
    SemNode * semCant;
    SemNode * semWrite;
} FileDescriptor;

typedef struct fd_node {
    FileDescriptor fd;
    struct fd_node * next;    
} NodeFd;

/* Create new FD struct and add to the list */
NodeFd * newFd(char * name);

/* Add STDIN, OUT and ERROR fd nodes to fd list */
void initFds();

/* Write on buffer given fd number */
void write(int fd, const char * buffer, int count);

/* Read from buffer given fd number */
void read(int fd, char * buffer, int count);

/* Add new FileDescriptor to the list */
NodeFd * addFdList(char* name);

/* Remove a node from the list */
void removeFdList(int fd);

/* Create new named Pipe */
int newPipe(char * name);

/* Opens an existing Pipe */
int openPipe(char * name);

/* Closes an existing Pipe */
void closePipe(int fd);

/* Prints all Pipes */
void showAllPipes();

#endif /* _FILEDESC_H_ */
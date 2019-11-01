#ifndef _PIPES_H_
#define _PIPES_H_

#include <fileDescriptors.h>

typedef enum {OPEN = 0, CLOSED} pipeState;

typedef struct pipe_node {
    NodeFd * fdNode;
    pipeState state;
    struct pipe_node * next;    
} NodePipe;

/* Create new named Pipe */
int newPipe(char * name);

/* Opens an existing Pipe */
int openPipe(char * name);

/* Closes an existing Pipe */
void closePipe(int fd);

/* Prints all Pipes */
void showAllPipes();

#endif /* _PIPES_H_ */



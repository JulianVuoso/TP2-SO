#include <fileDescriptors.h>

/*
typedef struct node {
    char name[BUFFER_SIZE];
    int fd;
    char * buffer;
    int read_index;
    int write_index; 
    Semaphore * sem;
} FileDescriptor;
*/

static NodeFd * first = 0;
static NodeFd * last = 0;

int newFd(int fd, char * name){
    NodeFd * nodefd = (NodeFd *) malloc(sizeof(NodeFd));
    nodefd->fd.name = name;
    nodefd->fd.fd = fd;
    
    return 0;
}

void write(int fd, char * buffer, int count){

}

void read(int fd, char * buffer, int count){

}


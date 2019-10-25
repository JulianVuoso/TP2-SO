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

typedef struct {
    FileDescriptor fd;
    FileDescriptor * next;    
} NodeFd;
*/

static int search(char * name);

static NodeFd * first = 0;
static NodeFd * last = 0;

int newFd(char * name){
    int resultFd = 0;
    if(first == 0){
        NodeFd * nodefd = (NodeFd *) malloc(sizeof(NodeFd));
        nodefd->fd.fd = resultFd;
        first = nodefd;
        last = nodefd;
    }
    else{
        if((resultFd = search(name)) == -1){
            NodeFd * nodefd = (NodeFd *) malloc(sizeof(NodeFd));
            nodefd->fd.name = name; 
            nodefd->fd.fd = last->fd.fd + 1;
            last->next = nodefd;
            last = nodefd;
        }
    }
    addFd(resultFd);
    return resultFd;
}

int search(char * name){
    NodeFd * aux = first;
    while(aux != 0){
        if (aux->fd.name == name)
            return aux->fd.fd;
        aux = aux->next;  
    } 
    return -1;
}

void write(int fd, char * buffer, int count){

}

void read(int fd, char * buffer, int count){

}


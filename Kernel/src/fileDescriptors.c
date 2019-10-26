#include <stdint.h>
#include <console.h>
#include <mutex.h>
#include <process.h>
#include <memoryManager.h>

#include <fileDescriptors.h>

/* Find fd in list */ 
static int searchName(char * name);
static NodeFd * searchFd(int fd);

/* If empty list, create first node */
static void initList(char* name);
/* If node not found, create and add before adding to process list */
static void addFdList(char* name);

/* Useful pointers to list ends */
static NodeFd * first = 0;
static NodeFd * last = 0;

/* Create new FD struct */
int newFd(char * name){
    int resultFd;
    if(first == 0)
        initList(name);
    else if((resultFd = searchName(name)) == -1)
            addFdList(name);
    addFd(resultFd);
    return resultFd;
}

/* Write on buffer given fd number */
void write(int fd, const char * buffer, int count){
    
    /* PARA PROBAR */
    if(fd == 1){
        print_N(buffer,count);
        return;
    }
    if(fd == 2){
        printError_N(buffer,count);
        return;
    }

    NodeFd * node = searchFd(fd);
    if(node == 0)    // Returns if FD not found
        return;
    
    /* Copy buffer in FD */
    waitSem(node->fd.sem);
    for(int i=0; i<count; i++)
        node->fd.buffer[i] = *(buffer++);
    postSem(node->fd.sem);
}

/* Read from buffer given fd number */
void read(int fd, char * buffer, int count){
    NodeFd * node = searchFd(fd);
    if(node == 0)    // Returns if FD not found      
        return;

    /* Copy buffer from FD */
    waitSem(node->fd.sem);     
    for(int i=0; i<count; i++)
        *(buffer++) = node->fd.buffer[i];
    postSem(node->fd.sem);
}


/* Initializes list of fds */
void initList(char* name){
    NodeFd * node = (NodeFd *) malloc(sizeof(NodeFd));
    node->fd.name = name; 
    node->fd.fd = 0;
    first = node;
    last = node;
}

/* Adds fd to list */
void addFdList(char* name){
    NodeFd * nodefd = (NodeFd *) malloc(sizeof(NodeFd));
    nodefd->fd.name = name; 
    nodefd->fd.fd = last->fd.fd + 1;
    nodefd->fd.sem = newSem(name, 1);
    last->next = nodefd;
    last = nodefd;
}

/* Search if the fd with name exists */
int searchName(char * name){
    NodeFd * aux = first;
    while(aux != 0){
        if (aux->fd.name == name)
            return aux->fd.fd;
        aux = aux->next;  
    } 
    return -1;
}

/* Search for a node given its fd */
NodeFd * searchFd(int fd){
    NodeFd * aux = first;
    while(aux != 0){
        if (aux->fd.fd == fd)
            return aux;
        aux = aux->next;  
    } 
    return 0;
}

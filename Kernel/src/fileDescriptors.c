#include <fileDescriptors.h>

/* Find fd in list */ 
static int searchName(char * name);
static NodeFd * searchFd(int fd);

/* If empty list, create first node */
static void initList(char* name);
/* If node not found, create and add before adding to process list */
static void addFdList(char* name);

static NodeFd * first = 0;
static NodeFd * last = 0;

int newFd(char * name){
    int resultFd;
    if(first == 0)
        initList(name);
    else if((resultFd = searchName(name)) == -1)
            addFdList(name);
    addFd(resultFd);
    return resultFd;
}

void initList(char* name){
    NodeFd * nodefd = (NodeFd *) malloc(sizeof(NodeFd));
    nodefd->fd.name = name; 
    nodefd->fd.fd = 0;
    first = nodefd;
    last = nodefd;
}

void addFdList(char* name){
    NodeFd * nodefd = (NodeFd *) malloc(sizeof(NodeFd));
    nodefd->fd.name = name; 
    nodefd->fd.fd = last->fd.fd + 1;
    last->next = nodefd;
    last = nodefd;
}

int searchName(char * name){
    NodeFd * aux = first;
    while(aux != 0){
        if (aux->fd.name == name)
            return aux->fd.fd;
        aux = aux->next;  
    } 
    return -1;
}

NodeFd * searchFd(int fd){
    NodeFd * aux = first;
    while(aux != 0){
        if (aux->fd.fd == fd)
            return aux;
        aux = aux->next;  
    } 
    return 0;
}

/* Write on buffer or Read from it, given fd number */
void write(int fd, char * buffer, int count){
    NodeFd * node = searchFd(fd);
    if(node == 0)
        return;
    for(int i=0; i<count; i++)
        node->fd.buffer[i] = *(buffer++);
}

void read(int fd, char * buffer, int count){
    NodeFd * node = searchFd(fd);
    if(node == 0)
        return;
    for(int i=0; i<count; i++)
        *(buffer++) = node->fd.buffer[i];
}


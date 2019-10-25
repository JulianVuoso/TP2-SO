#include <fileDescriptors.h>

static int searchName(char * name);
static int searchFd(int fd);

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
        if((resultFd = searchName(name)) == -1){
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

int searchName(char * name){
    NodeFd * aux = first;
    while(aux != 0){
        if (aux->fd.name == name)
            return aux->fd.fd;
        aux = aux->next;  
    } 
    return -1;
}

NodeFd * searcFd(int fd){
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
    NodeFd * node = searcFd(fd);
    if(node == 0)
        return;
    for(int i=0; i<count; i++)
        node->fd.buffer[i] = *(buffer++);
}

void read(int fd, char * buffer, int count){
    NodeFd * node = searcFd(fd);
    if(node == 0)
        return;
    for(int i=0; i<count; i++)
        *(buffer++) = node->fd.buffer[i];
}


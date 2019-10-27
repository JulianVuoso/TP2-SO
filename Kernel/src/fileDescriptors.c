#include <stdint.h>
#include <console.h>
#include <mutex.h>
#include <process.h>
#include <memoryManager.h>

#include <keyboard.h>
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

void initFds(){
    /* Initialize List with fds: 0 1 2 */
    initList("stdin");
    addFdList("stdout");
    addFdList("stderr");
}

/* Write on buffer given fd number */
void write(int fd, const char * buffer, int count){
    
    if(count == 0) return;

    NodeFd * node = searchFd(fd);
    if(node == 0)                   // Returns if FD not found
        return;

    waitSem(node->fd.sem);          // Waiting for main semaphore in this fd 

    if(fd < 3){
        switch (fd)
        {
            case 0: if (*buffer == -1 || (node->fd.count > 0 && node->fd.count < carRead()))
                        postSem(node->fd.semCant);
                    break;
            case 1: print_N(buffer, count); 
                    postSem(node->fd.sem); break;
            case 2: printError_N(buffer, count);            
                    postSem(node->fd.sem); break;
            default: break;
        }
    }
    else{    
        /* Copy buffer in FD */
        int eof = 0, i;
        // node->fd.count++;
        for(i = node->fd.write_index ; i < (count + node->fd.write_index) && eof == 0 ; i++){
            node->fd.buffer[i] = *(buffer++);
            if(*buffer == -1)
                eof = 1;                // Must leave the writing 
        }
        node->fd.write_index = i;
        // node->fd.count--;
        if (eof == 1 || (node->fd.count > 0 && node->fd.count < count)){
            postSem(node->fd.semCant);  // Posting secondary semaphore for sync
            // eof = 0;
        }
        postSem(node->fd.sem);
    }
}

/* Read from buffer given fd number */
void read(int fd, char * buffer, int count){
    if(count == 0) return;
    NodeFd * node = searchFd(fd);
    if(node == 0)                       // Returns if FD not found      
        return;

    static int eof = 0;                 // End Of File reception flag 
    waitSem(node->fd.sem);              // Waiting for main semaphore in this fd

    /* If characters not enough, increase variable and wait for them */
    if( (node->fd.write_index - node->fd.read_index) < count ){
        // node->fd.count++;     
        node->fd.count = count;
        waitSem(node->fd.semCant);
    }
    
    /* Copy buffer from FD if number of characters is enough */
    int i;
    for(i = node->fd.read_index; i < node->fd.read_index + count && eof == 0; i++){
        *(buffer++) = node->fd.buffer[i++ % BUFFER_SIZE];
        if(*buffer == -1)
            eof = 1;
    }
    node->fd.read_index = i;            // Update index in fd

    /*
    if(node->fd.count != 0 || eof != 0){           
        postSem(node->fd.semCant);      // Posting secondary semaphore for sync
        eof = 0;
    }*/

    /* Occasionally reset indexes */
    if(node->fd.read_index == node->fd.write_index){
        node->fd.read_index = 0;
        node->fd.write_index = 0;
    }

    postSem(node->fd.sem);              // Posting main semaphore in this fd

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
    nodefd->fd.semCant = newSem(name + '2', 0);
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

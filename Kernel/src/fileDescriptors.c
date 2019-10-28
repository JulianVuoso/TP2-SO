// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdint.h>
#include <console.h>
#include <mutex.h>
#include <process.h>
#include <memoryManager.h>
#include <keyboard.h>

#include <fileDescriptors.h>


#include <strings.h>

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

// /* Create new FD struct */
// int newFd(char * name){
//     int resultFd;
//     if(first == 0)
//         initList(name);
//     else if((resultFd = searchName(name)) == -1)
//             addFdList(name);
//     addFd(resultFd);
//     return resultFd;
// }

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

    waitSem(node->fd.semWrite);          // Waiting for main semaphore in this fd 

    if(fd < 3){
        switch (fd)
        {
            case 0: node->fd.buffer[node->fd.write_index++] = *(buffer);
                    if (*buffer == EOF || (node->fd.count > 0 && node->fd.count <= (node->fd.write_index - node->fd.read_index)))
                        postSem(node->fd.semCant);
                    // print("\n\t\tRecibi tecla: ");
                    // print_char(*buffer);
                    // print(". Estoy esperando llegar a %d y voy %d", node->fd.count, node->fd.write_index - node->fd.read_index);
                    break;
            case 1: print_N(buffer, count); 
                    break;
            case 2: printError_N(buffer, count);            
                    break;
            default: break;
        }
    }
    else{    
        /* Copy buffer in FD */
        int eof = 0, i;
        // node->fd.count++;
        for(i = node->fd.write_index ; i < (count + node->fd.write_index) && eof == 0 ; i++){
            node->fd.buffer[i] = *(buffer++);
            if(*buffer == EOF)
                eof = 1;                // Must leave the writing 
        }
        node->fd.write_index = i;
        // node->fd.count--;
        if (eof == 1 || (node->fd.count > 0 && node->fd.count < count)){
            postSem(node->fd.semCant);  // Posting secondary semaphore for sync
            // eof = 0;
        }
    }
    postSem(node->fd.semWrite);
}

/* Read from buffer given fd number */
void read(int fd, char * buffer, int count){
    if(count == 0) return;
    NodeFd * node = searchFd(fd);
    if(node == 0)                       // Returns if FD not found      
        return;

    // print("\n--Entre a read.");
    static int eof = 0;                 // End Of File reception flag 
    waitSem(node->fd.sem);              // Waiting for main semaphore in this fd

    // print("\n--Pase el primer semaforo.");

    /* If characters not enough, increase variable and wait for them */
    if( (node->fd.write_index - node->fd.read_index) < count ){
        // node->fd.count++;
        // print("\n--Seteo count.");
        node->fd.count = count;
        waitSem(node->fd.semCant);
    }
    
    // print("\n--Pase el segundo semaforo.");

    /* Copy buffer from FD if number of characters is enough */
    int i;
    for(i = node->fd.read_index; i < node->fd.read_index + count && eof == 0; i++){
        *buffer = node->fd.buffer[i % BUFFER_SIZE];
        buffer++;
        if(*buffer == EOF)
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

    node->fd.count = 0;
    postSem(node->fd.sem);              // Posting main semaphore in this fd

    // print("\n--Saliending.");
}

/* Initializes list of fds */
void initList(char* name){
    NodeFd * nodefd = (NodeFd *) malloc(sizeof(NodeFd));
    if (nodefd == 0) return; // No more memory
    nodefd->fd.name = (char *)malloc(strlen(name) + 1);
    stringcp(nodefd->fd.name, name);
    nodefd->fd.fd = 0;
    nodefd->fd.sem = newSem(name, 1);

    char aux[strlen(name) + 1], aux2[]="2", aux3[]="W";
    stringcp(aux, name);
    strcat(aux, aux2);
    nodefd->fd.semCant = newSem(aux, 0);
    stringcp(aux, name);
    strcat(aux, aux3);
    nodefd->fd.semWrite = newSem(aux, 1);

    nodefd->fd.count = 0;
    nodefd->fd.read_index = 0;
    nodefd->fd.write_index = 0;
    // nodefd->fd.buffer??????

    first = nodefd;
    last = nodefd;
}

/* Adds fd to list */
void addFdList(char* name){
    NodeFd * nodefd = (NodeFd *) malloc(sizeof(NodeFd));
    if (nodefd == 0) return; // No more memory
    nodefd->fd.name = (char *)malloc(strlen(name) + 1);
    stringcp(nodefd->fd.name, name);
    nodefd->fd.fd = last->fd.fd + 1;
    nodefd->fd.sem = newSem(name, 1);

    char aux[strlen(name) + 1], aux2[]="2", aux3[]="W";
    stringcp(aux, name);
    strcat(aux, aux2);
    nodefd->fd.semCant = newSem(aux, 0);
    stringcp(aux, name);
    strcat(aux, aux3);
    nodefd->fd.semWrite = newSem(aux, 1);

    nodefd->fd.count = 0;
    nodefd->fd.read_index = 0;
    nodefd->fd.write_index = 0;
    // nodefd->fd.buffer??????
    last->next = nodefd;
    last = nodefd;
}

/* Search if the fd with name exists */
int searchName(char * name){
    NodeFd * aux = first;
    while(aux != 0){
        if (stringcmp(aux->fd.name, name))
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

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
static NodeFd * searchName(char * name);
static NodeFd * searchFd(int fd);

// /* If empty list, create first node */
// static void initList(char* name);

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
    addFdList("stdin");
    addFdList("stdout");
    addFdList("stderr");
}

// ToDo: ver si funciona el error en -1 (si es que se puede)
/* Create new named Pipe. Returns -1 if error */
int newPipe(char * name) {
    print(name);

    /* Check if name already exists */
    if (searchName(name) != 0) return -1;

    print("\nBarrera");

    /* Add node to fileDescriptors list */
    NodeFd * node = addFdList(name);
    if (node == 0) return -1;
    node->fd.pipe = 1; // Indico que es un pipe
    int fd = node->fd.fd;

    print("\t\t--%d", fd);

    /* Add node to process fileDescriptors list */
    addFd(fd);

    return fd;
}

/* Opens an existing Pipe. Returns 0 if error */
int openPipe(char * name) {
    /* Check if name exists. If not, return */
    NodeFd * node = searchName(name);
    if (node == 0 || node->fd.pipe == 0) return -1;

    int fd = node->fd.fd;

    /* Add node to process fileDescriptors list */
    addFd(fd);

    return fd;
}

/* Closes an existing Pipe */
void closePipe(int fd) {
    /* Check if name exists. If not, return */
    NodeFd * node = searchFd(fd);
    if (node == 0 || node->fd.pipe == 0) return;

    /* Check if someone else is waiting to write */
    SemNode * sem = node->fd.semWrite;
    if (sem != 0 && sem->sem.blocked == 0){ // ToDo: check lock variable from mutex
        /* Send EOF signal to pipe */
        char aux = EOF;
        write(fd, &aux, 1);
    }

    /* Remove node from process fileDescriptors list */
    removeFd(fd);
}

/* Write on buffer given fd number */
void write(int fd, const char * buffer, int count){
    
    if(count == 0) return;

    int aliasFd = getAlias(fd);
    if (aliasFd < 0) return; // Returns if fd not listed

    NodeFd * node = searchFd(aliasFd);
    if(node == 0) return;    // Returns if FD not found

    // if (node->fd.fd >= 3) print("\n\n\t\t---Esperando primer sem\n\n");

    waitSem(node->fd.semWrite);          // Waiting for main semaphore in this fd 


    if(node->fd.fd < 3){
        switch (node->fd.fd)
        {
            case 0: node->fd.buffer[node->fd.write_index % BUFFER_SIZE] = *(buffer);
                    node->fd.write_index++;
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
            node->fd.buffer[i % BUFFER_SIZE] = *(buffer++);
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

    int aliasFd = getAlias(fd);
    if (aliasFd < 0) return; // Returns if fd not listed

    NodeFd * node = searchFd(aliasFd);
    if(node == 0) return;    // Returns if FD not found      

    // print("\n--Entre a read.");
    int eof = 0;                 // End Of File reception flag 
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
// void initList(char* name){
//     NodeFd * nodefd = (NodeFd *) malloc(sizeof(NodeFd));
//     if (nodefd == 0) return; // No more memory
//     nodefd->fd.name = (char *)malloc(strlen(name) + 1);
//     stringcp(nodefd->fd.name, name);
//     nodefd->fd.fd = 0;
//     nodefd->fd.sem = newSem(name, 1);

//     char aux[strlen(name) + 1], aux2[]="2", aux3[]="W";
//     stringcp(aux, name);
//     strcat(aux, aux2);
//     nodefd->fd.semCant = newSem(aux, 0);
//     stringcp(aux, name);
//     strcat(aux, aux3);
//     nodefd->fd.semWrite = newSem(aux, 1);

//     nodefd->fd.count = 0;
//     nodefd->fd.read_index = 0;
//     nodefd->fd.write_index = 0;

//     first = nodefd;
//     last = nodefd;
// }

/* Adds fd to list */
NodeFd * addFdList(char* name) {
    NodeFd * nodefd = (NodeFd *) malloc(sizeof(NodeFd));
    if (nodefd == 0) return 0; // No more memory
    nodefd->fd.name = (char *)malloc(strlen(name) + 1);
    stringcp(nodefd->fd.name, name);
    if (first == 0) {
        nodefd->fd.fd = 0;
        first = nodefd;
        last = nodefd;
    } else {
        nodefd->fd.fd = last->fd.fd + 1;
        last->next = nodefd;
        last = nodefd;
    }
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
    nodefd->fd.pipe = 0;
    
    return nodefd;
}

// ToDo: Ver si hay que liberar a todos los procesos o que hacemos
/* Remove a node from the list */
void removeFdList(int fd) {
    if (first == 0) return;

    NodeFd * aux = first;

    /* Check if its the first node */
    if (first->fd.fd == fd) {
        first = first->next;
        free(aux);
        return;
    }

    /* If first is not null and not the one to delete */
    while (aux->next != 0) {
        if (aux->next->fd.fd == fd) {
            NodeFd * toDelete = aux->next;
            aux->next = aux->next->next;
            free(toDelete);
            return;
        }
        aux = aux->next;
    }
}

/* Search if the fd with name exists */
NodeFd * searchName(char * name){
    NodeFd * aux = first;
    while(aux != 0){
        if (stringcmp(aux->fd.name, name))
            return aux;
        aux = aux->next;  
    } 
    return 0;
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

/* Prints all Pipes */
void showAllPipes() {
    /* If there are no pipes */
    if (first == 0) {
        print("\tThere are no Pipes created");
        return;
    }
    
    int printed = 0;
    NodeFd * iterator = first;
    print("\nName\t\tState\t\tBlocked Processes\n");
    while (iterator != 0) {
        if (iterator->fd.pipe) {
            print(iterator->fd.name); print("\t\t");
            printBlockedProcesses(iterator->fd.sem);
            printBlockedProcesses(iterator->fd.semWrite);
            printBlockedProcesses(iterator->fd.semCant);
            print("\n");
            printed = 1;
        }
        iterator = iterator->next;        
    }

    if (!printed) print("\tThere are no Pipes created");
}
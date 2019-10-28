// This is a peWsonal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code AnalyzeW for C, C++ and C#: http://www.viva64.com
#include <lib.h>
#include <scheduler.h>
#include <memoryManager.h>
#include <write.h>
#include <console.h>

/* Some static functions for list management */
static void addNodeW(uint64_t pid, const char * buff, uint64_t count);
static void removeNextW(NodeWrite * node);
static void removeFirstW();
static void writeSync(uint64_t fd, const char * buff, uint64_t count, char added);
//static void updateCurrentW();

/* Static variables */
static NodeWrite * addressW = 0;
static NodeWrite * firstW = 0;
static NodeWrite * lastW = 0;

static int writing = 0;

/* Manage list of pending writes, checking if resource is avaliable */
uint64_t write(uint64_t fd, const char * buff, uint64_t count) {
    char added = 0;
    if (count == 0) return 0;
    if (writing == 1 || firstW != 0){
        uint64_t pid = getPid();
        addNodeW(pid, buff, count);
        block(WRITE);
        added = 1;
    }
    writeSync(fd,buff,count, added);
    return 1;
}

/* Complete printing and set ready another if necessary */
static void writeSync(uint64_t fd, const char * buff, uint64_t count, char added){
    writing = 1;
    /* Print buff in fd */
    switch (fd) {
        case STDIN:
        case STDOUT: print_N(buff, count); break;
        case STDERR: printError_N(buff, count); break;
        default: break;
    }
    writing = 0;

    /* Remove done + Enable next node */
    if(added) removeFirstW();
    if(firstW != 0)
        setState(firstW->n.pid, READY);
}

/* Updates the values off all the waiting processes */
/*static void updateCurrentW() {
    if (addressW == 0 || firstW == 0) return;
    //(firstR->n.buff)[(firstR->n.index)++] = buffer[read_index++ % BUFFER_SIZE];
    if (firstW->n.index == firstW->n.count) {
        uint64_t pid = firstW->n.pid;      
        removeFirstW();
        setState(pid, READY);
    }
}*/

/* Memory manageW for the nodes */
static NodeWrite * newNodeW();
static void freeNodeW(NodeWrite * node);
static void cleanMemW();

/* Removes a node given its pid */
void removeNodeW(uint64_t pid) {
    if (addressW == 0 || firstW == 0) return;

    /* If its the first one */
    if (firstW->n.pid == pid) removeFirstW();

    NodeWrite * aux;
    for (aux = firstW; aux->n.next != 0 && aux->n.next->n.pid != pid; aux = aux->n.next);
    if (aux->n.next == 0) return; // Not found
    removeNextW(aux);
}

/* Removes first node */
static void removeFirstW() {
    NodeWrite * aux = firstW;
    if (lastW == firstW) lastW = 0;
    firstW = firstW->n.next;
    freeNodeW(aux);
}

/* Removes the next node of the given one */
static void removeNextW(NodeWrite * node) {
    NodeWrite * freeAux = node->n.next;
    if (freeAux == lastW) lastW = node;
    node->n.next = node->n.next->n.next;
    freeNodeW(freeAux);
}

/* Adds a Node to the begining of the list */
static void addNodeW(uint64_t pid, const char * buff, uint64_t count) {
    if (addressW == 0) {
        addressW = (NodeWrite *)malloc(SIZE);
        cleanMemW();  
    }
    NodeWrite * node = newNodeW();
    node->n.pid = pid;
    node->n.buff = buff;
    node->n.count = count;
    node->n.next = 0;

    if (firstW == 0) firstW = node;
    else lastW->n.next = node;
    lastW = node;
}

/* Returns direction of a new Node */
static NodeWrite * newNodeW() {
    for (uint64_t i = 0; i < SIZE / sizeof(NodeWrite); i++) {
        if ((addressW+i)->n.used == 0) {
            (addressW+i)->n.used = 1;
            return addressW+i;
        }
    }
    return 0;    
}

/* Frees the Node given */
static void freeNodeW(NodeWrite * node) {
    node->n.used = 0;
}

/* Sets all available places to free */
static void cleanMemW() {
    for (uint64_t i = 0; i < SIZE / sizeof(NodeWrite); i++)
        (addressW+i)->n.used = 0;    
}
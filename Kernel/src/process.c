// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <mutex.h>
#include <lib.h>
#include <scheduler.h>
#include <stack.h>
#include <memoryManager.h>
#include <console.h>
#include <timelib.h>
#include <strings.h>

#include <process.h>

/* Add FD with Alias to list while creating process */
static fdPointer * addFdAlias(int inAlias, int outAlias);

static uint64_t c_pid = 0;
// static fdPointer * first = 0;

/* Creates a new process and adds it to scheduler */
uint64_t create(void * entryPoint, char * name, level context, int inAlias, int outAlias) {
    Process data = createNoSched(entryPoint, name, context, inAlias, outAlias);
    /* Add process to scheduler */
    // print("n\tScheduler: ");
    add(data);
    if (context == FORE && data.pid > 1)
        block(0);
        // block(SCREEN);
    return data.pid;
}

/* Creates a new process */
Process createNoSched(void * entryPoint, char * name, level context, int inAlias, int outAlias) {
    // print("\n\tPROCESS.C\tIN: %d, OUT: %d", inAlias, outAlias);
    // print("\n\tStack: ");
    void * processStack = malloc(STACK_SIZE);
    if (processStack == 0) { // ERROR --> NO HAY MAS MEMORIA --> VER QUE DEVUELVO
        // return 0;
    }

    /* Creates stack for process */
    char * lastAddress = (char *) get_last_address(processStack);
    char * stackBase = lastAddress - sizeof(uint64_t);
    StackFrame stack = initStack(entryPoint, (void *) stackBase);
    memcpy(lastAddress - sizeof(StackFrame), &stack, sizeof(StackFrame));
    
    /* Creates process data */
    Process data;
    // print("\tName: ");
    data.name = (char *)malloc(strlen(name) + 1);
    stringcp(data.name, name);
    data.pid = c_pid++;
    data.ppid = getPid();
    data.sp = (uint64_t) lastAddress - sizeof(StackFrame);
    data.bp = (uint64_t) stackBase;
    data.priority = 3;
    data.context = context;
    data.state = READY;
    data.stack = processStack;
    data.sem = 0;
    data.first = addFdAlias(0, inAlias);
    if (data.first != 0) data.first->next = addFdAlias(1, outAlias);

    return data;
}

/* Deletes process */
void remove(Process p) {
    freeResources(p);
    free(p.stack);
    free(p.name);
    fdPointer * aux = p.first;
    while (aux != 0){
        fdPointer * aux2 = aux;
        aux = aux->next;
        free(aux2);
    }
}

/* Frees all the resources used by the process */
void freeResources(Process p) {
    if (p.state != BLOCKED) return;
    if (p.sem == 0) removeNodeT(p.pid);
    else deallocateSem(p.sem, p.pid);    
}

/* Add new file descriptor to list */
fdPointer * addFd(int fd){

    /* Create aux Process to modify fd list */
    Process p = getCurrent()->process;

    fdPointer * iterator = p.first;
    fdPointer * prev = iterator;
    while (iterator != 0) {
        if (iterator->fd == fd)
            return 0;
        prev = iterator;
        iterator = iterator->next;
    }

    fdPointer * fdp = (fdPointer *) malloc(sizeof(fdPointer));
    if (fdp == 0) return 0; // No more memory 
    fdp->fd = fd;
    fdp->alias = fd;
    fdp->next = 0;

    /* Start list or add Node to it */
    if(p.first == 0)
        p.first = fdp;
    else
        prev->next = fdp;
    getCurrent()->process = p;
    return fdp;
}

/* Remove file descriptor from list */
void removeFd(int fd) {

    /* Create aux Process to modify fd list */
    Process p = getCurrent()->process;
    if (p.first == 0) return;

    fdPointer * iterator = p.first;

    /* Check if its the first node */
    if (p.first->fd == fd) {
        p.first = p.first->next;
        free(iterator);
        return;
    }

    /* If first is not null and not the one to delete */
    while (iterator->next != 0) {
        if (iterator->next->fd == fd) {
            fdPointer * toDelete = iterator->next;
            iterator->next = iterator->next->next;
            free(toDelete);
            return;
        }
        iterator = iterator->next;
    }

    getCurrent()->process = p;
}

/* Return realFd for the current process, or -1 if not listed */
int getAlias(int fd) {
    /* Create aux Process to modify fd list */
    Process p = getCurrent()->process;
    fdPointer * iterator = p.first;
    while (iterator != 0) {
        if (iterator->fd == fd)
            return iterator->alias;
        iterator = iterator->next;
    }
    return -1;
}

/* Print process stack */
void printProcessStack(Process p) {
    char * lastAddress = (char *) get_last_address(p.stack);
    print("\nProcess %d \n", p.pid);
    uint64_t * aux;
    for (aux = (uint64_t *) p.sp; aux < (uint64_t *) lastAddress; aux++) {
        print("- Address: 0x");
        printHex((uint64_t)aux);
        print(" - Content: 0x");
        printHex(*aux);
        print("\n");
    }
    print("\n-----------------------\n");
}

// /* Adds the alias for stdin and stdout */
// static fdPointer * addFdAlias(int inAlias, int outAlias){
//     // print("\tInFd: ");
//     fdPointer * in = (fdPointer *) malloc(sizeof(fdPointer));
//     if (in == 0) return 0; // No more memory
//     // print("\tOutFd: ");
//     fdPointer * out = (fdPointer *) malloc(sizeof(fdPointer));
//     if (out == 0){
//         free((void *) in);
//         return 0;
//     }  // No more memory

//     /* Generate Node of STDIN FD with given alias */
//     in->fd = 0;
//     in->alias = inAlias;
//     in->next = out;

//     /* Generate Node of STDOUT FD with given alias after in Node in list */
//     out->fd = 1;
//     out->alias = outAlias;
//     out->next = 0;
    
//     /* Return Node direction for being inserted in Process List */
//     return in; 
// }

static fdPointer * addFdAlias(int oldFd, int newFd) {
    fdPointer * ptr = (fdPointer *) malloc(sizeof(fdPointer));
    if (ptr == 0) return 0; // No more memory

    // print("\n\t\tFD: %d, ALIAS: %d", oldFd, newFd);
    ptr->fd = oldFd;
    ptr->alias = newFd;
    ptr->next = 0;

    return ptr;
}

// AGREGAR QUE NO PUEDAN CORTAR AL PHYLO
void sigInt() {
    if (getPid() > 1)
        killCurrent();
}

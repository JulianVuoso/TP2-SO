#include <process.h>
#include <stack.h>
#include <scheduler.h>
#include <memoryManager.h>

#include <console.h>
#include <mutex.h>
static fdPointer * addFdAlias(int inAlias, int outAlias);

static uint64_t c_pid = 0;
static fdPointer * first = 0;

uint64_t create(void * entryPoint, char * name, level context, int inAlias, int outAlias) {
    Process data = createNoSched(entryPoint, name, context, inAlias, outAlias);
    /* Add process to scheduler */
    add(data);

    return data.pid;
}

/*
    typedef struct{
        int fd;
        int alias;
        fdPointer * next;
    }fdPointer;
*/

fdPointer * addFd(int fd){
    Process p = getCurrent()->n.process;
    fdPointer * fdp = (fdPointer *) malloc(sizeof(fdPointer));
    fdp->fd = fd;
    if(p.first == 0)
        p.first = fdp;
    else
    {
        fdPointer * aux = first;
        first = fdp;
        fdp->next = aux;
    }
    getCurrent()->n.process = p;
    return fdp;
}

fdPointer * addFdAlias(int inAlias, int outAlias){
    fdPointer * in = (fdPointer *) malloc(sizeof(fdPointer));
    fdPointer * out = (fdPointer *) malloc(sizeof(fdPointer));
    in->fd = 0;
    in->alias = inAlias;
    in->next = out;
    out->fd = 1;
    out->alias = outAlias;
    return in;
}

Process createNoSched(void * entryPoint, char * name, level context, int inAlias, int outAlias) {
    void * processStack = malloc(STACK_SIZE);
    if (processStack == 0) { // ERROR --> NO HAY MAS MEMORIA --> VER QUE DEVUELVO
        // return 0;
    }

    /* Creates stack for process */
    char * lastAddress = (char *) getLastAddress(processStack);
    char * stackBase = lastAddress - sizeof(uint64_t);
    StackFrame stack = initStack(entryPoint, (void *) stackBase);
    memcpy(lastAddress - sizeof(StackFrame), &stack, sizeof(StackFrame));
    
    /* Creates process data */
    Process data;
    data.name = name;
    data.pid = c_pid++;
    data.sp = (uint64_t) lastAddress - sizeof(StackFrame);
    data.bp = (uint64_t) stackBase;
    data.priority = 3;
    data.context = context;
    data.state = READY;
    data.stack = processStack;
    data.sem = 0;

    data.first = addFdAlias(inAlias, outAlias);

    /* Prints result on console */
    //printProcessStack(data);
    return data;
}

void remove(Process p) {
    freeResources(p);
    free(p.stack);
}

void freeResources(Process p) {
    if (p.state != BLOCKED) return;
    deallocateSem(p.sem, p.pid);    
}

void printProcessStack(Process p) {
    char * lastAddress = (char *) getLastAddress(p.stack);
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
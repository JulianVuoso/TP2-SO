#include <process.h>

/* Add FD with Alias to list while creating process */
static fdPointer * addFdAlias(int inAlias, int outAlias);

static uint64_t c_pid = 0;
static fdPointer * first = 0;

/* Creates a new process and adds it to scheduler */
uint64_t create(void * entryPoint, char * name, level context, int inAlias, int outAlias) {
    Process data = createNoSched(entryPoint, name, context, inAlias, outAlias);
    /* Add process to scheduler */
    add(data);
    return data.pid;
}

/* Creates a new process */
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

    return data;
}

/* Deletes process */
void remove(Process p) {
    freeResources(p);
    free(p.stack);
}

/* Frees all the resources used by the process */
void freeResources(Process p) {
    if (p.state != BLOCKED) return;
    if (p.sem == 0) return;
    if (p.sem == -1) removeNodeT(p.pid);
    else deallocateSem(p.sem, p.pid);    
}

/* Add new file descriptor to list */
fdPointer * addFd(int fd){

    /* Create aux Process for being built with FD list and then charged again */
    Process p = getCurrent()->process;
    fdPointer * fdp = (fdPointer *) malloc(sizeof(fdPointer));  
    fdp->fd = fd;

    /* Start list or add Node to it */
    if(p.first == 0)
        p.first = fdp;
    else {
        fdPointer * aux = first;
        first = fdp;
        fdp->next = aux;
    }
    getCurrent()->process = p;
    return fdp;
}

/* Print process stack */
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

/* Adds the alias for stdin and stdout */
static fdPointer * addFdAlias(int inAlias, int outAlias){
    fdPointer * in = (fdPointer *) malloc(sizeof(fdPointer));
    fdPointer * out = (fdPointer *) malloc(sizeof(fdPointer));

    /* Generate Node of STDIN FD with given alias */
    in->fd = 0;
    in->alias = inAlias;
    in->next = out;

    /* Generate Node of STDOUT FD with given alias after in Node in list */
    out->fd = 1;
    out->alias = outAlias;
    
    /* Return Node direction for being inserted in Process List */
    return in; 
}

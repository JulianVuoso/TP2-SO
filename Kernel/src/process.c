#include <process.h>
#include <stack.h>
#include <scheduler.h>
#include <memoryManager.h>

#include <console.h>

static uint64_t c_pid;

static void printData(StackFrame stack, Process p, char * lAddress) {
    print("PROCESS STACK: 0x");
    printHex((uint64_t)p.stack);
    print("\n");
    print("PROCESS STACK BASE: 0x");
    printHex(stack.sp);
    print("\n");
    print("PROCESS STACK TOP: 0x");
    printHex(stack.sp - sizeof(StackFrame));
    print("\n");
    print("STACK SIZE: %d\n", sizeof(StackFrame));
    uint64_t * aux;
    for (aux = (uint64_t *) p.sp; aux < (uint64_t *) lAddress; aux++) {
        print("- Address: 0x");
        printHex((uint64_t)aux);
        print("- Content: 0x");
        printHex(*aux);
        print("\n");
    }
}

Process create(void * entryPoint, char * name) {
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
    data.pid = (uint64_t) c_pid++;
    data.sp = (uint64_t) lastAddress - sizeof(StackFrame);
    data.bp = (uint64_t) stackBase;
    data.priority = 0;
    data.context = FORE;
    data.state = READY;
    data.stack = processStack;

    /* Add process to scheduler */
    if (add(data) == 0) {
        // ERROR 
    }

    /* Prints result on console */
    printData(stack, data, lastAddress);
    
    return data;
}

void remove(Process p) {
    free(p.stack);
}
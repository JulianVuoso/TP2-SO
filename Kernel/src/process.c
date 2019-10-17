#include <process.h>
#include <stack.h>
#include <memoryManager.h>

#include <console.h>

// PARA QUE ME SIRVE EL NOMBRE??
Process create(void * entryPoint, char * processName) {
    void * processStack = malloc(STACK_SIZE);
    if (processStack == 0) { // ERROR --> NO HAY MAS MEMORIA --> VER QUE DEVUELVO
        
        // return 0;
    }
    char * lastAddress = (char *) getLastAddress(processStack);
    char * stackBase = lastAddress - sizeof(uint64_t);
    StackFrame stack = initStack(entryPoint, (void *) stackBase);
    memcpy(lastAddress - sizeof(StackFrame), &stack, sizeof(StackFrame));
    Process data;
    // CAMBIAR DATOS
    data.pid = (uint64_t) stackBase;
    data.priority = 0;
    data.state = READY;
    data.sp = (uint64_t) lastAddress - sizeof(StackFrame);

    print("PROCESS STACK: 0x");
    printHex((uint64_t)processStack);
    print("\n");
    print("PROCESS STACK BASE: 0x");
    printHex(stack.sp);
    print("\n");
    print("PROCESS STACK TOP: 0x");
    printHex((uint64_t)stackBase - sizeof(StackFrame));
    print("\n");
    print("STACK SIZE: %d\n", sizeof(StackFrame));
    uint64_t * aux;
    for (aux = (uint64_t *) data.sp; aux < (uint64_t *) lastAddress; aux++) {
        print("- Address: 0x");
        printHex((uint64_t)aux);
        print("- Content: 0x");
        printHex(*aux);
        print("\n");
    }
    return data;
}

void kill(uint64_t pid) {
    

}

uint64_t getPid() {

}

/***************************************************
    Process.h
****************************************************/

#ifndef _PROCESS_H_
#define _PROCESS_H_

#include <stdint.h>
#include <lib.h>

#define STACK_SIZE	3000

enum processState {Ready, Running, Blocked};

typedef struct Process {
    uint64_t pid;
    uint64_t sp; // Es uint64_t o void *?
    uint8_t priority;
    enum processState state;
} Process;

Process createProcess(void * entryPoint, char * processName);
void deleteCurrentProcess();

#endif /* _PROCESS_H_ */
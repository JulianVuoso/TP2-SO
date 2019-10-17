/***************************************************
    Process.h
****************************************************/

#ifndef _PROCESS_H_
#define _PROCESS_H_

#include <stdint.h>
#include <lib.h>

#define STACK_SIZE	3000

typedef enum {READY, RUNNING, BLOCKED} states;

typedef enum {FORE, BACK} level;

typedef struct {
    char * name;
    uint64_t pid;
    uint64_t sp; // Es uint64_t o void *?
    uint64_t bp;
    uint8_t priority;
    level context;
    states state; // VA aca???
} Process;

/* Creates a new process */
Process create(void * entryPoint, char * processName);

/* Deletes process with the given pid */
void remove(Process p);

/* Returns current process pid */
uint64_t getPid();

#endif /* _PROCESS_H_ */
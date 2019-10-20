/***************************************************
    Process.h
****************************************************/

#ifndef _PROCESS_H_
#define _PROCESS_H_

#include <stdint.h>
#include <lib.h>

#define STACK_SIZE	3000

typedef enum {READY = 0, RUNNING, BLOCKED, UNDEFINED} states;

typedef enum {FORE = 0, BACK} level;

typedef struct {
    char * name;
    uint64_t pid;
    uint64_t sp;
    uint64_t bp;
    uint8_t priority;
    level context;
    states state;

    void * stack;
} Process;

/* Creates a new process */
uint64_t create(void * entryPoint, char * name);

Process createNoSched(void * entryPoint, char * name);

/* Deletes process */
void remove(Process p);

/* Print process stack */
void printProcessStack(Process p);

#endif /* _PROCESS_H_ */
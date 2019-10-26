/***************************************************
                    Process.h
****************************************************/

#ifndef _PROCESS_H_
#define _PROCESS_H_

#include <mutex.h>

#define STACK_SIZE	3000

typedef enum {READY = 0, RUNNING, BLOCKED, UNDEFINED} states;

typedef enum {NONE = 0, TIME, READ, WRITE, SCREEN} resources;

typedef enum {FORE = 0, BACK} level;

typedef struct nodeFdp{
    int fd;
    int alias;
    struct nodeFdp * next;
} fdPointer;

typedef struct {
    char * name;
    uint64_t pid;
    uint64_t ppid;
    uint64_t sp;
    uint64_t bp;
    uint8_t priority;
    level context;
    states state;
    SemNode * sem;
    void * stack;
    fdPointer * first;
} Process;

/* Creates a new process and adds it to scheduler */
uint64_t create(void * entryPoint, char * name, level context, int inAlias, int outAlias);

/* Creates a new process */
Process createNoSched(void * entryPoint, char * name, level context, int inAlias, int outAlias);

/* Deletes process */
void remove(Process p);

/* Frees all the resources used by the process */
void freeResources(Process p);

/* Add new file descriptor to list */
fdPointer * addFd(int fd);

/* Print process stack */
void printProcessStack(Process p);

#endif /* _PROCESS_H_ */
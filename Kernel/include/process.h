/***************************************************
    Process.h
****************************************************/

#ifndef _PROCESS_H_
#define _PROCESS_H_

#include <stdint.h>
#include <lib.h>

#define STACK_SIZE	3000

typedef enum {READY = 0, RUNNING, BLOCKED, UNDEFINED} states;

typedef enum {NONE = 0, TIME, READ, WRITE} resources;

typedef enum {FORE = 0, BACK} level;

typedef struct {
    char * name;
    uint64_t pid;
    uint64_t sp;
    uint64_t bp;
    uint8_t priority;
    level context;
    states state;
    resources resource;
    void * stack;
} Process;

typedef struct{
    int fd;
    int alias;
}fdPointer;

/* Creates a new process */
uint64_t create(void * entryPoint, char * name, level context);

Process createNoSched(void * entryPoint, char * name, level context);

/* Deletes process */
void remove(Process p);

/* Print process stack */
void printProcessStack(Process p);

/* Frees all the resources used by the process */
void freeResources(Process p);

/* Add new file descriptor to list */
void addFd(int fd);

/* Make alias from file descriptor */
void dup(int fd, int alias);

#endif /* _PROCESS_H_ */
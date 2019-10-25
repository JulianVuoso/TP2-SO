/***************************************************
                    Mutex.h
****************************************************/

#ifndef _MUTEX_H_
#define _MUTEX_H_

#include <stdint.h>
#include <process.h>

typedef struct wait_node {
    uint64_t pid;
    struct wait_node * next; 
} WaitNode;

typedef struct sem {
    char * name;
    uint64_t count;
    char lock;
    WaitNode * owner; 
} Semaphore;

typedef struct sem_node {
    Semaphore sem;
    struct sem_node * next;
} SemNode;

/* Create new named Semaphore */
SemNode * newSem(char * name, uint64_t init);

/* Opens an existing semaphore */
SemNode * open(char * name);

/* Delete NodeSem and ready next in list */
void post(SemNode * sem);

/* Add NodeSem to list and block process */
void wait(SemNode * sem);

/* Function deallocates system resources allocated 
for the calling process for this semaphore */
void close(SemNode * sem);

/* Prints all semaphores */
void showAll();

#endif /* _MUTEX_H_ */
/***************************************************
                    mutex.h
****************************************************/

#ifndef _MUTEX_H_
#define _MUTEX_H_

#include <stdint.h>

typedef struct wait_node {
    uint64_t pid;
    struct wait_node * next; 
} WaitNode;

typedef struct sem_data {
    char * name;
    uint64_t count;
    WaitNode * blocked;
    WaitNode * last;
} Semaphore;

typedef struct sem_node {
    Semaphore sem;
    struct sem_node * next;
} SemNode;

/* Create new named Semaphore */
SemNode * newSem(char * name, uint64_t init);

/* Opens an existing semaphore */
SemNode * openSem(char * name);

/* Delete NodeSem and ready next in list */
void postSem(SemNode * sem);

/* Add NodeSem to list and block process */
void waitSem(SemNode * sem);

/* Function deallocates system resources allocated 
for the calling process for this semaphore */
void closeSem(SemNode * sem);

/* Deallocates system resources of the process */
void deallocateSem(SemNode * sem, uint64_t pid);

/* Prints all semaphores */
void showAllSem();

#endif /* _MUTEX_H_ */
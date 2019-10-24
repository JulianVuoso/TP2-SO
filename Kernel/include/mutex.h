/***************************************************
                    Mutex.h
****************************************************/

#ifndef _MUTEX_H_
#define _MUTEX_H_

#include <stdint.h>
#include <process.h>

typedef struct node {
    char * name;
    int count;
    char lock;
    NodeSem * owner; 
} Semaphore;

typedef struct node {
    int pid;
    NodeSem * next; 
} NodeSem;

/* statics */
/* Create new Semaphore Node */
/* Lock Sem */
/* Change owner */

/* Create new Semaphore */
Semaphore * newSem(char * name);

/* Create new Semaphore with  */
Semaphore * newSemCount(char * name, int initCount);

/* Delete NodeSem and ready next in list */
void post(char * name);

/* Add NodeSem to list and block process */
void wait(char * name);

/* Remove Semaphore */
void remove(char * name);

#endif /* _MUTEX_H_ */
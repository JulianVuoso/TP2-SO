/***************************************************
                    Scheduler.h
****************************************************/

#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_


#include <stdint.h>
#include <lib.h>
#include <timelib.h>
#include <memoryManager.h>
#include <console.h>
#include <interrupts.h>
#include <moduleAddresses.h>
#include <mutex.h>
#include <process.h>

#define MAX_PRIO 3
#define SIZE 4000

typedef struct node_sched{
    int used;
    int times;
    Process process;
    struct node_sched * next;
} Node;

/* Creates the scheduler */
void initScheduler();

/* Chooses next process to give the CPU */
uint64_t scheduler(uint64_t sp);

/* Adds a new process to the scheduler */
uint8_t add(Process p);

/* Kills current process */
void killCurrent();

/* Deletes a process given PID */
uint64_t kill(uint64_t pid);

/* Sets priority of the process given its PID */
uint64_t setPriority(uint64_t pid, uint8_t n);

/* Get the state of the process given its PID */
states getState(uint64_t pid);

/* Sets the state of the process given its PID */
uint64_t setState(uint64_t pid, states state);

/* Blocks current process for using sem */
uint64_t block(SemNode * sem);

/* Returns current process pid */
uint64_t getPid();

/* Checks if a process is loaded */
int checkLoaded();

/* Return current process */
Node * getCurrent();

/* Prints all processes */
void listAll();

#endif /* _SCHEDULER_H_ */
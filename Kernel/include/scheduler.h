/***************************************************
    Scheduler.h
****************************************************/

#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#include <stdint.h>
#include <process.h>

/* Chooses next process to give the CPU */
uint64_t scheduler(uint64_t sp);

/* Adds a new process to the scheduler */
uint8_t add(Process p);

/* Deletes a process given PID */
uint8_t kill(uint64_t pid);

/* Sets priority of the process given its PID */
uint8_t setPriority(uint64_t pid, uint8_t n);

/* Sets the state of the process given its PID */
uint8_t setState(uint64_t pid, states state);

/* Prints all processes */
void listAll();

Node * search(uint64_t pid);

#endif /* _SCHEDULER_H_ */
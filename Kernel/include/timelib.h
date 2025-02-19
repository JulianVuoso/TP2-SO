/***************************************************
                    TimeLib.h
****************************************************/

#ifndef _TIMELIB_H_
#define _TIMELIB_H_

#include <stdint.h>

#define SIZE 4000
#define TICK_LENGTH     55  // Cantidad de ms que equivalen a un tick

typedef struct node_sleep {
    uint8_t used;
    uint64_t pid;
    uint64_t time;
    struct node_sleep * next;
} NodeTime;

/* Handles timer */
void timer_handler();

/* Count of elapsed ticks */
int ticks_elapsed();

/* Seconds elapsed */
int seconds_elapsed();

/* Syscall calls here */
void sleep(uint64_t millis);

/* Removes the node from the list given its pid */
void removeNodeT(uint64_t pid);


#endif

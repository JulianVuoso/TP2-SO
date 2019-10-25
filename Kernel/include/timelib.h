/***************************************************
                    TimeLib.h
****************************************************/

#ifndef _TIMELIB_H_
#define _TIMElib_H_

#include <stdint.h>
#include <lib.h>

#define SIZE 4000
#define TICK_LENGTH     55  // Cantidad de ms que equivalen a un tick

typedef union NodeTime {
    struct {
        uint8_t used;
        uint64_t pid;
        uint64_t time;
        union NodeTime * next;
    } n;
    long x; // Here happens the align 
} NodeTime;


void timer_handler();
int ticks_elapsed();
int seconds_elapsed();
void sleep(uint64_t millis);

/* Removes the node from the list given its pid */
void removeNodeT(uint64_t pid);


#endif

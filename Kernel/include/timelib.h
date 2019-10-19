#ifndef _TIMELIB_H_
#define _TIMElib_H_

#include <stdint.h>
#include <lib.h>

#define SIZE 4000
#define TICK_LENGTH     55  // Cantidad de ms que equivalen a un tick

<<<<<<< HEAD
typedef union NodeTime {
=======
typedef union nodeTime {
>>>>>>> 6a1553f90b604653a32078e072849f4351a5a6c8
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

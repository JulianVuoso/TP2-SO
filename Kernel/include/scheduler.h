/***************************************************
    Scheduler.h
****************************************************/

#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#include <stdint.h>
#include <process.h>

uint64_t scheduler(uint64_t sp);

void setFirst(Process p);

void setSecond(Process p);

void setGo();

#endif /* _SCHEDULER_H_ */
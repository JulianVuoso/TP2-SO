#include <mutex.h>

/* statics */
/* Create new Semaphore Node */
/* Lock Sem */
/* Change owner */

Semaphore * newSem(char * name);
Semaphore * newSemCount(char * name, int initCount);
void post(char * name);
void wait(char * name);

Semaphore * newSem(char * name){
    Semaphore * sem;
    sem->name = name;
    sem->lock = 0;
    sem->owner = 0;  

}


#include <mutex.h>
#include <memoryManager.h>
#include <scheduler.h>
#include <lib.h>

static SemNode * list = 0;

/* Create new Semaphore */
SemNode * newSem(char * name, uint64_t init) {
    
    Semaphore sem;
    sem.name = malloc(stringlen(name));
    stringcp(sem.name, name);
    sem.owner = 0;

    if (init == 0) sem.lock = 1;
    else sem.lock = 0;

    if (init >= 0) sem.count = init;
    else sem.count = 1;
    
    SemNode * node = malloc(sizeof(SemNode));
    node->next = 0;
    node->sem = sem;

    if (list != 0) node->next = list;
    list = node;

    return list;
}

/* Opens an existing semaphore */
SemNode * open(char * name) {
    SemNode * iterator = list;
    while (iterator != 0) {
        if (stringcmp(name, iterator->sem.name)) return iterator;
        iterator = iterator->next;
    }
    return 0;    
}

/* Delete NodeSem and ready next in list */
void post(SemNode * sem) {

}

/* Add NodeSem to list and block process */
void wait(SemNode * sem) {

}

/* Function deallocates system resources allocated 
for the calling process for this semaphore */
void close(SemNode * sem) {

}

/* Prints all semaphores */
void showAll() {

}

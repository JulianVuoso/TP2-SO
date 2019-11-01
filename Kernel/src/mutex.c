// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdint.h>
#include <process.h>
#include <memoryManager.h>
#include <scheduler.h>
#include <lib.h>
#include <strings.h>
#include <console.h>

#include <mutex.h>

static int searchSem(SemNode * sem);

static SemNode * list = 0;

/* Create new Semaphore */
SemNode * newSem(char * name, uint64_t init) {
    
    /* If the semaphore already exists returns 0 */
    if (list != 0 && openSem(name) != 0) return 0;

    /* Creates Semaphore */
    Semaphore sem;
    sem.name = (char *)malloc(strlen(name) + 1);
    stringcp(sem.name, name);
    sem.blocked = 0;
    sem.last = 0;

    /* Checks if init has a valid value, default = 1 */
    //ToDo Si es uint64_t, es al pedo ver si es >= 0
    if (init >= 0) sem.count = init;
    else sem.count = 1;
    
    /* Creates node */
    SemNode * node = (SemNode *)malloc(sizeof(SemNode));
    if (node == 0) return 0; // No more memory
    node->next = 0;
    node->sem = sem;

    /* Appends node to begining of the list */
    if (list != 0) node->next = list;
    list = node;
    
    return list;
}

/* Opens an existing semaphore */
SemNode * openSem(char * name) {
    SemNode * iterator = list;
    while (iterator != 0) {
        if (stringcmp(name, iterator->sem.name)) return iterator;
        iterator = iterator->next;
    }
    return 0;    
}

static int searchSem(SemNode * sem) {
    SemNode * iterator = list;
    while (iterator != 0) {
        if (iterator == sem) return 1;
        iterator = iterator->next;
    }
    return 0;
}

/* Delete NodeSem and ready next in list */
void postSem(SemNode * sem) {
    // if (!stringcmp(sem->sem.name, "stdoutW")){
    //     print("\n##POST: Sem count: %d del semaforo %s. Sem BLOCKED esta en 0x", sem->sem.count, sem->sem.name);
    //     printHex((uint64_t)sem->sem.blocked);
    // }
    
    /* If address does not correspond to a semaphore, exit */
    if (!searchSem(sem)) return;

    /* When count is > 0 or no process in list */
    if (sem->sem.count > 0 || sem->sem.blocked == 0) {
        atom_swap(&(sem->sem.count), sem->sem.count + 1);
        // if (!stringcmp(sem->sem.name, "stdoutW")) print("\n##POST: Sem count: %d del semaforo %s", sem->sem.count, sem->sem.name);
        return;
    }

    /* If count 0 and we have blocked processes */
    WaitNode * p = sem->sem.blocked;
    // print("\nSem: %s\t##SemBlo valia: 0x", sem->sem.name);
    // printHex((uint64_t) sem->sem.blocked);
    // print("\t##Con next: 0x");
    // printHex((uint64_t) sem->sem.blocked->next);
    sem->sem.blocked = sem->sem.blocked->next;
    setState(p->pid, READY);
    // print("\t##Liber: 0x");
    // printHex((uint64_t) p);

    // print("\t##SemBlo quedo: 0x");
    // printHex((uint64_t) sem->sem.blocked);
    free((void *)p);
}

/* Add NodeSem to list and block process */
void waitSem(SemNode * sem) {
    // if (!stringcmp(sem->sem.name, "stdoutW")) print("\n##WAIT: Sem count: %d del semaforo %s", sem->sem.count, sem->sem.name);
    
    /* If address does not correspond to a semaphore, exit */
    if (!searchSem(sem)) return;

    /* When count is >= 1 do not block or add to list */
    if (sem->sem.count >= 1) {
        atom_swap(&(sem->sem.count), sem->sem.count - 1);
        // if (!stringcmp(sem->sem.name, "stdoutW")) print("\n##WAIT: Sem count: %d del semaforo %s", sem->sem.count, sem->sem.name);
        return;
    }

    /* If count is 0 */
    /* Create node to add */
    WaitNode * node = (WaitNode *)malloc(sizeof(WaitNode));
    if (node == 0) return; // No more memory
    node->pid = getPid();
    node->next = 0;

    // print("\nSem: %s\t##Mal: 0x", sem->sem.name);
    // printHex((uint64_t) node);
    
    // print("\t##SemBlo valia: 0x");
    // printHex((uint64_t) sem->sem.blocked);

    // print("\t##SemLast valia: 0x");
    // printHex((uint64_t) sem->sem.last);
    /* Add node to the list */
    if (sem->sem.blocked == 0) sem->sem.blocked = node;
    else sem->sem.last->next = node;
    sem->sem.last = node;

    // print("\t##SemBlo quedo: 0x");
    // printHex((uint64_t) sem->sem.blocked);
    // print("\t##Con next: 0x");
    // printHex((uint64_t) sem->sem.blocked->next);

    /* Block the current process with sem */
    block(sem);
}

/* Deallocates system resources allocated 
for the calling process for this semaphore */
void closeSem(SemNode * sem) {
    deallocateSem(sem, getPid());
}

/* Deallocates system resources of the process */
void deallocateSem(SemNode * sem, uint64_t pid) {
    WaitNode * curr = sem->sem.blocked;
    WaitNode * prev = 0;

    /* Empty list */
    if (curr == 0) return;

    /* Search */
    while (curr != 0 && curr->pid != pid) {
        prev = curr;
        curr = curr->next;
    }

    /* If not found */
    if (curr == 0) return;

    /* Found */
    if (prev == 0) sem->sem.blocked = curr->next;
    else prev->next = curr->next;
    if (curr->next == 0) sem->sem.last = prev;
    free(curr);
}

/* Prints all semaphores */
void showAllSem() {
    /* If there are no semaphores */
    if (list == 0) {
        print("\tThere are no Semaphores created");
        return;
    }
    
    SemNode * iterator = list;
    print("\nName\t\tState\t\tCount\n");
    while (iterator != 0) {
        print(iterator->sem.name); print("\t\t"); 
        print((iterator->sem.count == 0) ? "Locked" : "Unlocked"); print("\t\t");
        print("%d", iterator->sem.count); print("\n");
        iterator = iterator->next;
    }
}

/* Print Blocked processes from a semaphore */
void printBlockedProcesses(SemNode * sem) {
    if (sem == 0) return;
    
    WaitNode * iterator = sem->sem.blocked;
    while (iterator != 0) {
        print("%d", iterator->pid); print("\t");
        iterator = iterator->next;
    }
}
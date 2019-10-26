// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdint.h>
#include <lib.h>
#include <interrupts.h>
#include <memoryManager.h>
#include <scheduler.h>
#include <process.h>

#include <timelib.h>

/* Some static functions for list management */
static void addNodeT(uint64_t pid, uint64_t time);
static void updateList();

/* static variables */
static NodeTime * firstT = 0;
static unsigned long ticks = 0;

/* Handles timer */
void timer_handler() {
	ticks++;
    updateList();
}

/* Count of elapsed ticks */
int ticks_elapsed() {
	return ticks;
}

/* Seconds elapsed */
int seconds_elapsed() {
	return ticks / 18;
}

/* Syscall calls here */
void sleep(uint64_t millis) {
    if (millis < 55) return;
    uint64_t pid = getPid();
    addNodeT(pid, millis);
    block(0);
}

/* Updates the values off all the waiting processes */
static void updateList() {
    NodeTime * iterator = firstT;
    NodeTime * prev = firstT;

    /* We iterate on list */
    while (iterator != 0) {

        /* We free the process */
        if (--(iterator->time) == 0) {
            NodeTime * next = iterator->next;
            setState(iterator->pid, READY);
            free(iterator);

            /* First node */
            if (prev->next == next) {
                firstT = next;
                prev = next;
            } else prev->next = next;
            iterator = next;

        } else {
            /* Normal iteration */
            prev = iterator;
            iterator = iterator->next;
        }        
    }
}

/* Removes the next node of the given one */
static void removeNextT(NodeTime * node) {
    NodeTime * freeAux = node->next;
    node->next = node->next->next;
    free(freeAux);
}

/* Removes a node given its pid */
void removeNodeT(uint64_t pid) {
    if (firstT == 0) return;

    NodeTime * aux = firstT;

    /* If its the first one */
    if (firstT->pid == pid) {
        firstT = firstT->next;
        free(aux);
        return;
    }

    for (aux = firstT; aux->next != 0 && aux->next->pid != pid; aux = aux->next);
    if (aux->next == 0) return; // Not found
    removeNextT(aux);
}

/* Adds a Node to the begining of the list */
static void addNodeT(uint64_t pid, uint64_t time) {
   
    NodeTime * node = (NodeTime *)malloc(sizeof(NodeTime));
    node->pid = pid;
    node->time = time / TICK_LENGTH;
    node->next = 0;

    if (firstT != 0) node->next = firstT;    
    firstT = node;
}
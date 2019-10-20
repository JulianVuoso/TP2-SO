// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdint.h>
#include <interrupts.h>
#include <memoryManager.h>
#include <scheduler.h>

#include <timelib.h>

/* Some static functions for list management */
static void addNodeT(uint64_t pid, uint64_t time);
static void removeNextT(NodeTime * node);
static void removeFirstT();
static void updateListT();

/* static variables */
static NodeTime * addressT = 0;
static NodeTime * firstT = 0;
static unsigned long ticks = 0;

void timer_handler() {
	ticks++;
    updateListT();
}

int ticks_elapsed() {
	return ticks;
}

int seconds_elapsed() {
	return ticks / 18;
}

void sleep(uint64_t millis) {
    if (millis < 55) return;
    uint64_t pid = getPid();
    addNodeT(pid, millis);
    block(TIME);
}

/* Updates the values off all the waiting processes */
static void updateListT() {
    if (addressT == 0) return;
    NodeTime * prev = firstT;
    for (NodeTime * aux = firstT; aux != 0; aux = aux->n.next) {
        if (--(aux->n.time) == 0) {
            uint64_t pid = aux->n.pid;
            if (prev == aux) removeFirstT();
            else removeNextT(prev);
            setState(pid, READY);
        }
        prev = aux;
    }
}

/* Memory manager for the nodes */
static NodeTime * newNodeT();
static void freeNodeT(NodeTime * node);
static void cleanMemT();

/* Removes a node given its pid */
void removeNodeT(uint64_t pid) {
    if (addressT == 0 || firstT == 0) return;

    /* If its the first one */
    if (firstT->n.pid == pid) removeFirstT();

    NodeTime * aux;
    for (aux = firstT; aux->n.next != 0 && aux->n.next->n.pid != pid; aux = aux->n.next);
    if (aux->n.next == 0) return; // Not found
    removeNextT(aux);
}

/* Removes first node */
static void removeFirstT() {
    NodeTime * aux = firstT;
    firstT = firstT->n.next;
    freeNodeT(aux);
}

/* Removes the next node of the given one */
static void removeNextT(NodeTime * node) {
    NodeTime * freeAux = node->n.next;
    node->n.next = node->n.next->n.next;
    freeNodeT(freeAux);
}

/* Adds a Node to the begining of the list */
static void addNodeT(uint64_t pid, uint64_t time) {
    if (addressT == 0) {
        addressT = (NodeTime *)malloc(SIZE);
        cleanMemT();  
    }
    NodeTime * node = newNodeT();
    node->n.pid = pid;
    node->n.time = time / TICK_LENGTH;
    node->n.next = 0;

    if (firstT != 0) node->n.next = firstT;    
    firstT = node;
}

/* Returns direction of a new Node */
static NodeTime * newNodeT() {
    for (uint64_t i = 0; i < SIZE / sizeof(NodeTime); i++) {
        if ((addressT+i)->n.used == 0) {
            (addressT+i)->n.used = 1;
            return addressT+i;
        }
    }
    return 0;    
}

/* Frees the Node given */
static void freeNodeT(NodeTime * node) {
    node->n.used = 0;
}

/* Sets all available places to free */
static void cleanMemT() {
    for (uint64_t i = 0; i < SIZE / sizeof(NodeTime); i++)
        (addressT+i)->n.used = 0;    
}
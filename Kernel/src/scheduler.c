#include <scheduler.h>
#include <lib.h>
#include <timelib.h>
#include <memoryManager.h>
#include <console.h>
#include <interrupts.h>
#include <moduleAddresses.h>
#include <mutex.h>

/* Static search of a node */
static Node *search(uint64_t pid);

static Node *current;
static Node *haltProcess;

static int init;

/* Gives control to processes on list   */
uint64_t scheduler(uint64_t sp) {
    /* Only count timer if scheduler was not forced or in Halt */
    if (current == 0 || current->process.state != BLOCKED || init == 3) timer_handler();

    /* Start case scenario, do not use sp given */
    switch(init){
        case 0: return sp; 
        case 1: init = 2; current->process.state = RUNNING; break;
        case 2: current->process.sp = sp; break;
        case 3: haltProcess->process.sp = sp; break;
        default: init = 3; return haltProcess->process.sp;
    }

    /* If current process wasn't blocked and did not reach times */
    if(++(current->times) != pow(2, MAX_PRIO - current->process.priority) && current->process.state != BLOCKED)
        return current->process.sp;
    
    /* Change current process */
    current->times = 0;
    if (current->process.state != BLOCKED)
        current->process.state = READY;
    
    /* Search for the next non blocked process on list */
    Node * aux = current;
    do {
        current = current->next;
    } while (current->process.state != READY && aux->process.pid != current->process.pid);

    /* If no processes to be run, run haltProcess */
    if (current->process.state != READY) {
        init = 3;
        return haltProcess->process.sp;
    }

    /* Run found process */
    init = 2;
    current->process.state = RUNNING;
    return current->process.sp;
}

/* Initializes the scheduler */
void initScheduler() {
    init = 0;
    current = 0;    
    
    /* Initializes the halt process */
    Process aux = createNoSched(idleModuleAddress, "IDLE", FORE, 0, 0);
    haltProcess = (Node *)malloc(sizeof(Node));
    haltProcess->times = 0;
    haltProcess->process = aux;
    haltProcess->next = haltProcess;
}

/* Adds the process p to shceduler */
uint8_t add(Process p) {
    /* Create new node */
    Node * node = (Node *)malloc(sizeof(Node));
    node->times = 0;
    node->process = p;
    node->next = node;

    /* Add to list */
    if (current == 0) {  
        current = node;
        init = 1;
    } else {
        node->next = current->next;
        current->next = node;
    }
    return 0;
}

/* Kills current process */
void killCurrent() {
    kill(getPid());
}

/* Deletes a process given pid */
uint64_t kill(uint64_t pid) {
    if (current == 0 || pid < 2)
        return 0;

    /* Search of the process */
    Node * iterator = current;
    do {
        /* Found */
        if (iterator->next->process.pid == pid) {
            /* The only process */
            if (current == current->next) {
                remove(current->process);
                free(current);
                current = 0;
                init = 0;
                halt();
            }

            Node * toDelete = iterator->next; 
            iterator->next = toDelete->next;
            /* If is the current process */
            if (toDelete == current) {
                current = toDelete->next;
                init = 1;
            }
            remove(toDelete->process);
            free(toDelete);
            return pid;  
        }
        iterator = iterator->next;
    } while (iterator != current);                 
}

/* Sets priority of the process given its PID */
uint64_t setPriority(uint64_t pid, uint8_t n) {
    if (n > MAX_PRIO || n < 0 ) return 2;
    Node * node = search(pid);
    if (node == 0) return 1;
    node->process.priority = n;
    return 0;
}

/* Get the state of the process given its PID */
states getState(uint64_t pid) {
    Node * node = search(pid);
    if (node == 0) return UNDEFINED;
    return node->process.state;
}

/* Sets the state of the process given its PID */
uint64_t setState(uint64_t pid, states state) {
    if (state == RUNNING) return 1;
    Node * node = search(pid);
    if (node == 0) return 1;

    /* If not the one currently running */
    if (node->process.pid != current->process.pid) {
        node->process.state = state;
        node->process.sem = 0;
        return 0;
    }

    /* If a running process us set to READY */
    if (current->process.state == RUNNING && state == READY) return 1;

    /* If we blocked the current process */
    node->process.state = state;
    return 0;
}

/* Blocks current process using a sem */
uint64_t block(SemNode * sem) {
    if (current == 0) return 1;
    current->process.state = BLOCKED;
    current->process.sem = sem;
    force_timer_tick();
    return 0;
}

/* Returns current process pid */
uint64_t getPid() {
    return current->process.pid;
}

/* Searches for a node given its pid */
static Node * search(uint64_t pid) {
    if (current == 0) return 0;
    Node * node = current;
    do {
        if (pid == node->process.pid)
            return node;
        node = node->next;
    } while (node != current);
    return 0;
}

/* Checks if a process is loaded */
int checkLoaded() {
    return current != 0 && current->process.state == RUNNING;
}

/* Return current process */
Node * getCurrent(){
    return current;
}

/* Prints all processes */
void listAll() {
    if (current == 0) {
        print("NOP\t-1\tThere is no Process in the scheduler");
        return;
    }

    Node * node = current;
    print("\nName\tPID\tSP\t\tBP\tPrio\tLevel\t\tState\n");
    do {
        Process p = node->process;
        char* stat = p.state==0? "Ready" : (p.state==1? "Running":"Blocked");
        char* lvl = p.context==0? "Foreground" : "Background";
        print(p.name); print("\t"); 
        printHex(p.pid); print("\t");
        printHex(p.sp); print("\t"); 
        printHex(p.bp); print("\t"); 
        printHex((uint64_t)p.priority); print("\t"); 
        print(lvl); print("\t"); 
        print(stat); print("\n");
        node = node->next;
    } while (node != current);
    print("\n");
}
#include <scheduler.h>
#include <timelib.h>
#include <memoryManager.h>
#include <console.h>
#include <interrupts.h>

static Node *search(uint64_t pid);
static Node * newNode();
static void freeNode(Node * node);
static void cleanMem();
static uint64_t pow(int base, int exponent);

static Node *current;
static Node *address;

static Node *haltProcess;

static int init;

uint64_t scheduler(uint64_t sp) {
    timer_handler();

    // si se mata ulyimo proceso porque termino, entonces sp que devuelvo es donde
    // ira el programa cuando no tiene procesos para correr?
    // habria que crear una especie de proceso idle
    // no puede no haber procesos

    switch(init){
        case 0: return sp; 
        case 1: init = 2; current->n.process.state = RUNNING; break;
        case 2: current->n.process.sp = sp; break;
        default: haltProcess->n.process.sp = sp; break;
    }
        
    current->n.times++;
    if(current->n.times == pow(2, MAX_PRIO - current->n.process.priority) || current->n.process.state == BLOCKED || current->n.process.state == UNDEFINED){
        current->n.times = 0;
        if (current->n.process.state != BLOCKED && current->n.process.state != UNDEFINED)
            current->n.process.state = READY;
        
        // ToDo Ver si todo esto de aca abajo se va o no
        Node * aux = current;
        // print("\nCURRENT: %s, estado %d",current->n.process.name, current->n.process.state);
        do {
            current = current->n.next;
        } while (current->n.process.state != READY && aux->n.process.pid != current->n.process.pid);

        // Chequeo si pego la vuelta, no hay ninguno para ejecutar
        if (current->n.process.state != READY) {
            // halt();
            init = 3;
            return haltProcess->n.process.sp;
        } else {
            init = 2;
            current->n.process.state = RUNNING;
        }
    }
    return current->n.process.sp;
}

uint8_t add(Process p) {
    if (address == 0) return 0;
    if(init == 0){
        init = 1;
    }

    Node * node = newNode();
    node->n.times = 0;
    node->n.process = p;           
    if (current == 0) {
        current = node;
        node->n.next = node;
    } else {
        Node * aux;
        aux = current->n.next;
        current->n.next = node;
        node->n.next = aux;
    }
    return 0;
}

void killCurrent() {
    kill(getPid());
}

uint64_t kill(uint64_t pid) {
    if (current == 0)
        return 0;
    // ToDo Ver si esto de aca abajo se va o no
    // if (pid <= 1)
    //     return 0;

    /* If its the only process */
    if (current == current->n.next) {
        if (current->n.process.pid != pid)
            return 0;
        Node * aux = current; 
        uint64_t pid = aux->n.process.pid;
        remove(aux->n.process);
        freeNode(aux);
        current = 0;
        init = 0;
        // ToDo Ver si esto de aca abajo se va o no
        halt();
        return pid;
    }
    Node * node = current;
    do {
        if (pid == node->n.next->n.process.pid) {
            Node * aux = node->n.next; 
            uint64_t pid = aux->n.process.pid;
            node->n.next = aux->n.next;
            if (current == aux) {
                current = aux->n.next;
                init = 1;
            }
            remove(aux->n.process);
            freeNode(aux);
            return pid;
        }
        node = node->n.next;
    } while (node != current); // si el siguiente no lo vi aun
    return 0;               
}

uint64_t setPriority(uint64_t pid, uint8_t n) {
    if (n > MAX_PRIO || n < 0 ) return 2;
    Node * node = search(pid);
    if (node == 0) return 1;
    node->n.process.priority = n;
    return 0;
}

states getState(uint64_t pid) {
    Node * node = search(pid);
    if (node == 0) return UNDEFINED;
    return node->n.process.state;
}

uint64_t setState(uint64_t pid, states state) {
    if (state == RUNNING) return 1;
    Node * node = search(pid);
    if (node == 0) return 1;
    /* If not the one currently running */
    if (node->n.process.pid != current->n.process.pid) {
        node->n.process.state = state;
        return 0;
    }
    if (current->n.process.state == RUNNING && state == READY) return 1;

    /* If we blocked the current process */
    node->n.process.state = state;
    return 0;
}

uint64_t getPid() {
    return current->n.process.pid;
}

Node * search(uint64_t pid) {
    if (current == 0) return 0;
    Node * node = current;
    do {
        if (pid == node->n.process.pid)
            return node;
        node = node->n.next;
    } while (node != current);
    return 0;
}

void listAll() {
    Node * node = current;
    print("\nName\tPID\tSP\t\tBP\tPrio\tLevel\t\tState\n");
    if (current == 0) {
        print("NOP\t-1\tThere is no Process in the scheduler");
        return;
    }
    do {
        Process p = node->n.process;
        char* stat = p.state==0? "Ready" : (p.state==1? "Running":"Blocked");
        char* lvl = p.context==0? "Foreground" : "Background";
        print(p.name); print("\t"); printHex(p.pid); print("\t");
        printHex(p.sp); print("\t"); printHex(p.bp); print("\t"); printHex((uint64_t)p.priority);
        print("\t"); print(lvl); print("\t"); print(stat); print("\n");
        node = node->n.next;
    } while (node != current);
    print("\n");
}

static void * const idleModuleAddress = (void*)0x1400000;

void initScheduler() {
    init = 0;
    current = 0;
    address = (Node *)malloc(SIZE);
    cleanMem();
    // create(idleModuleAddress, "IDLE");
    Process aux = createNoSched(idleModuleAddress, "IDLE");
    setState(aux.pid, UNDEFINED);
    haltProcess = newNode();
    haltProcess->n.times = 0;
    haltProcess->n.process = aux;
    haltProcess->n.next = haltProcess;
}

int checkLoaded() {
    return current != 0 && current->n.process.state == RUNNING;
}

/* Memory manager for the nodes */
/* Returns direction of a new Node */
static Node * newNode() {
    for (uint64_t i = 0; i < SIZE / sizeof(Node); i++) {
        if ((address+i)->n.used == 0) {
            (address+i)->n.used = 1;
            return address+i;
        }
    }
    return 0;    
}

/* Frees the Node given */
static void freeNode(Node * node) {
    node->n.used = 0;
}

/* Sets all available places to free */
static void cleanMem() {
    for (uint64_t i = 0; i < SIZE / sizeof(Node); i++)
        (address+i)->n.used = 0;    
}

static uint64_t pow(int base, int exponent) {
	int result = 1;
	for (uint64_t i = 0; i < exponent; i++){
		result = result * base;
	}
	return result;
}
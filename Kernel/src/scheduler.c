#include <scheduler.h>
#include <time.h>

static Node * current;

typedef struct {
    int times;
    Process process;
    Node * next;
} Node;

uint64_t scheduler(uint64_t sp) {
     
}

uint8_t add(Process p) {
    Node * node;            // HACER MALLOC
    if(current == 0)        
        current = node;
    else{
        Node aux;
        aux = current->next;
        current->next = node;
        node->next = &aux;
    }
}

uint8_t kill(uint64_t pid) {
    Node * node = current;
    uint64_t first = node->process.pid;
    do {
        if(pid == node->next->process.pid){    // si es el de adelante
            remove(node->next->process);
            // free del nodo
            node->next = node->next->next;     // lo puenteo
            return 1;                          
        }
        node = node->next;
    } while(node->next->pid != first);         // si el siguiente no lo vi aun 
    return -1;                                 // No existe
}

uint8_t setPriority(uint64_t pid, uint8_t n) {

}

uint8_t setState(uint64_t pid, states state) {
    Node * node = search(pid);
}

Node * search(uint64_t pid) {
   Node * node = 0;
    uint64_t first = node->process.pid;
    do {
        if(pid == node->next->process.pid)    // si es el de adelante
            return node;                     
        node = node->next;
    } while(node->next->pid != first);
    return 0;
}

void listAll() {
    
}


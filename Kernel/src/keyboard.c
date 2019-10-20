// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <lib.h>
#include <scheduler.h>
#include <memoryManager.h>

#include <keyboard.h>

/* Some static functions for list management */
static void addNodeR(uint64_t pid, char * buff, uint64_t count);
static void removeNextR(NodeRead * node);
static void removeFirstR();
static void updateCurrentR();

/* Static variables */
static NodeRead * addressR = 0;
static NodeRead * firstR = 0;
static NodeRead * currentR = 0;
static NodeRead * prevR = 0;

char buffer[BUFFER_SIZE];
unsigned int save_index = 0;
unsigned int read_index = 0;

static unsigned char map[90]={'`','\e','1','2','3','4','5','6','7','8','9','0','-','=','\b','\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',
        0/*CTRL*/,'a','s','d','f','g','h','j','k','l',';','\'', 0, 'S','\\','z','x','c','v','b','n','m',',','.','/','S',0,0/*ALT*/,' ',
        'M',0,0,0,0,0,0,0,0,0,0,0,0,0,UP_KEY,0,0,LEFT_KEY,0,RIGHT_KEY,0,0,DOWN_KEY };

static unsigned char shift_map[90]={'~','\e','!','@','#','$','%','^','&','*','(',')','_','+','\b','\t','q','w','e','r','t','y','u','i','o','p','{','}','\n',
        0/*CTRL*/,'a','s','d','f','g','h','j','k','l',':','\"', 0, 'S','|','z','x','c','v','b','n','m','<','>','?','S',0,0/*ALT*/,' ',
        'M',0,0,0,0,0,0,0,0,0,0,0,0,0,UP_KEY,0,0,LEFT_KEY,0,RIGHT_KEY,0,0,DOWN_KEY };

// ESC = \e
// BACK = \b
// SHIFT = S
// CAPS = M
// CTRL = C
// ALT = A
// UP = U
// LEFT = L
// RIGHT = R
// DOWN = D

// Solo chequeo si presione SHIFT o MAY, los demas pasan como vienen
void keyboard_handler() {
    static int caps = 1, shift = 1, release_caps = 1;
    unsigned char key = read_port(0x60);

    if ((key >= 90 && key < 0x80) || key >= 0x80 + 90)
        return;

    unsigned char car;
    if (key < 0x80){ // Aprieto tecla
        if (shift == 1)
            car = map[key];
        else
            car = shift_map[key];
        switch (car){
            case 'S':   
                shift = -1;
                break;
            case 'M':   
                if (release_caps){
                    caps *= -1;
                    release_caps = 0;
                }
                break;
            default:
                if (shift * caps == -1){ // Uno de los dos activos, paso a mayus
                    buffer[save_index % BUFFER_SIZE] = toUpper(car);
                    //printChar(toUpper(car), 0x0F);
                } else{
                    buffer[save_index % BUFFER_SIZE] = car;
                    //printChar(car, 0x0F);
                }
                save_index++;
                updateCurrentR();
                break;
        }
    } else { // Suelto tecla
        car = map[key - 0x80];
        switch (car){
            case 'S':
                shift = 1;
                break;
            case 'M':
                release_caps = 1;
                break;
        }
    }
}

char toUpper(char car){
    if (car >= 'a' && car <= 'z')
        car -= ('a' - 'A');
    return car;
}

#include <console.h>

/* WE ARE NOT USING FD */
uint64_t read(uint64_t fd, char * buff, uint64_t count) {
    if (count == 0) return 0;
    if (firstR == 0 && (save_index - read_index) >= count) {
        int i = 0;
        for (i = 0; i < count; i++) {
            buff[i] = buffer[read_index % BUFFER_SIZE];
            read_index++;
        }
        if (read_index == save_index) {
            read_index = 0;
            save_index = 0;
        }
    } else {
        uint64_t pid = getPid();
        addNodeR(pid, buff, count);
        setState(pid, BLOCKED);
    }
}

/* Updates the values off all the waiting processes */
static void updateCurrentR() {
    if (addressR == 0 || firstR == 0) return;
    if (currentR == 0) {
        currentR = firstR;
        prevR = firstR;
    }
    (currentR->n.buff)[(currentR->n.index)++] = buffer[read_index % BUFFER_SIZE];
    read_index++;

    if (currentR->n.index == currentR->n.count) {
        uint64_t pid = currentR->n.pid;
        if (currentR == prevR) removeFirstR();
        else removeNextR(prevR);
        setState(pid, READY);
    }
    prevR = currentR;
    currentR = currentR->n.next;
}

/* Memory manager for the nodes */
static NodeRead * newNodeR();
static void freeNodeR(NodeRead * node);
static void cleanMemR();

/* Removes a node given its pid */
void removeNodeR(uint64_t pid) {
    if (addressR == 0 || firstR == 0) return;

    /* If its the first one */
    if (firstR->n.pid == pid) removeFirstR();

    NodeRead * aux;
    for (aux = firstR; aux->n.next != 0 && aux->n.next->n.pid != pid; aux = aux->n.next);
    if (aux->n.next == 0) return; // Not found
    removeNextR(aux);
}

/* Removes first node */
static void removeFirstR() {
    NodeRead * aux = firstR;
    firstR = firstR->n.next;
    freeNodeR(aux);
}

/* Removes the next node of the given one */
static void removeNextR(NodeRead * node) {
    NodeRead * freeAux = node->n.next;
    node->n.next = node->n.next->n.next;
    freeNodeR(freeAux);
}

/* Adds a Node to the begining of the list */
static void addNodeR(uint64_t pid, char * buff, uint64_t count) {
    if (addressR == 0) {
        addressR = (NodeRead *)malloc(SIZE);
        cleanMemR();  
    }
    NodeRead * node = newNodeR();
    node->n.pid = pid;
    node->n.buff = buff;
    node->n.count = count;
    node->n.index = 0;
    node->n.next = 0;

    if (firstR != 0) node->n.next = firstR;    
    firstR = node;
}

/* Returns direction of a new Node */
static NodeRead * newNodeR() {
    for (uint64_t i = 0; i < SIZE / sizeof(NodeRead); i++) {
        if ((addressR+i)->n.used == 0) {
            (addressR+i)->n.used = 1;
            return addressR+i;
        }
    }
    return 0;    
}

/* Frees the Node given */
static void freeNodeR(NodeRead * node) {
    node->n.used = 0;
}

/* Sets all available places to free */
static void cleanMemR() {
    for (uint64_t i = 0; i < SIZE / sizeof(NodeRead); i++)
        (addressR+i)->n.used = 0;    
}
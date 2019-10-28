#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#define BUFFER_SIZE 50

#define UP_KEY  24
#define LEFT_KEY  27
#define RIGHT_KEY  26
#define DOWN_KEY  25

#define SIZE 4000

typedef union NodeRead {
    struct {
        uint8_t used;
        uint64_t pid;
        char * buff;
        uint64_t count;
        uint64_t index;
        union NodeRead * next;
    } n;
    long x; // Here happens the align 
} NodeRead;

void keyboard_handler();
char toUpper(char car);
uint64_t read(uint64_t fd, char * buff, uint64_t count);

/* Removes a node given its pid */
void removeNodeR(uint64_t pid);

#endif
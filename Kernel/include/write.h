#ifndef _WRITE_H_
#define _WRITE_H_

#define STDIN   0
#define STDOUT  1
#define STDERR  2

typedef union NodeWrite {
    struct {
        uint8_t used;
        uint64_t pid;
        const char * buff;
        uint64_t count;
        union NodeWrite * next;
    } n;
    long x; // Here happens the align 
} NodeWrite;


uint64_t write(uint64_t fd, const char * buff, uint64_t count);

/* Removes a node given its pid */
void removeNodeW(uint64_t pid);

#endif
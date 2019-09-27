// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <memoryManager.h>
#include <lib.h>


typedef struct mem_header {
    uint8_t * start;

    uint64_t total;
    uint64_t ocupied;
    uint64_t free;
} header;

typedef struct mem_node {
    // pointer to the next free memory block node
    uint8_t * next;

    // pointer to real memory
    uint8_t * address;

    // quantity of consecutive free pages
    uint64_t size;
    
} node;

int create_manager(uint8_t * address, uint64_t pageSize, uint64_t maxPages) {
    
}

void * malloc(uint64_t size) {

}

void free(void * ptr) {

}

void status(uint64_t * total, uint64_t * ocupied, uint64_t * free) {

}




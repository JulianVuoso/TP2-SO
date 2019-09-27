#include <lib.h>
typedef header * list;

typedef struct mem_header {
    uint8_t * start;

    uint64_t total;
    uint64_t ocupied;
    uint64_t free;
    uint64_t pageSize;
} header;

typedef struct mem_node {
    // pointer to the next free memory block node
    uint8_t * next;

    // index of the first page of the free block
    uint64_t index;

    // quantity of consecutive free pages
    uint64_t size;
    
} node;

list newList(uint8_t * adress, uint64_t pageSize, uint64_t maxPages) {
    node first = {0, 0, maxPages};    
    header aux = { &first, maxPages, 0, maxPages, pageSize};
    return &aux;   
}
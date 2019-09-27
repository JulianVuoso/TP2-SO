// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <memoryManager.h>
#include <lib.h>

static header memory;

typedef struct mem_header {
    uint8_t * freeList;
    uint8_t * usedList;
 
    //  measured in pages
    uint64_t total;
    uint64_t ocupied;
    uint64_t free;

    // page size in bytes
    uint64_t pageSize;
} header;

typedef struct mem_node {
    // pointers to the next and previous memory block nodes
    uint8_t * next;
    uint8_t * prev;

    // pointer to start of the block (node)
    uint8_t * address;

    // quantity of consecutive free pages
    uint64_t size;
} node;

int create_manager(uint8_t * address, uint64_t pageSize, uint64_t maxPages) {
    // check for errors
    if (address + pageSize * maxPages > MAX_DIR) return -1;   
    
    // initialize list header
    memory.total = memory.free = maxPages;
    memory.freeList = address;
    memory.pageSize = pageSize;

    // create first block of maxPages pages
    node first = {0, 0, address, maxPages};
    memcpy(address, &first, sizeof(node));

    return 0;
}


void * malloc(uint64_t bytes) {

}

void free(void * ptr) {

}

void status(uint64_t * total, uint64_t * occupied, uint64_t * free) {
    *total = memory.total * memory.pageSize;
    *occupied = memory.occupied * memory.pageSize;
    *free = memory.free * memory.pageSize;
}




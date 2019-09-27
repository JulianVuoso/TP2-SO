// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <memoryManager.h>
#include <lib.h>

static void merge_next(node * block);

// header of the memory manager
static header memory;

typedef struct mem_node {
    // pointers to the next and previous memory block nodes
    struct mem_node * next;
    struct mem_node * prev;

    // pointer to start of the block (node)
    uint8_t * address;

    // quantity of consecutive free pages
    uint64_t size;
} node;

typedef struct mem_header {
    node * freeList;
    node * usedList;
 
    //  measured in pages
    uint64_t total;
    uint64_t ocupied;
    uint64_t free;

    // page size in bytes
    uint64_t pageSize;
} header;

int create_manager(uint8_t * address, uint64_t pageSize, uint64_t maxPages) {
    // check for errors
    if (address + pageSize * maxPages > MAX_DIR) return -1;   
    
    // initialize list header
    memory.total = memory.free = maxPages;
    memory.freeList = (node *)address;
    memory.pageSize = pageSize;

    // create first block of maxPages pages
    node first = {0, 0, address, maxPages};
    memcpy(address, &first, sizeof(node));

    return 0;
}

void * malloc(uint64_t bytes) {
    // convertir a nimero de paginas sumandole aLiGNEMnte node

    // recorro nodos hasta encontrar uno de tamano >= a lo que quiero

    // creo nodo nueo que apunte comienzo siguiente bloque si me sobra espacio

    // lo conecto al anterior si me sobrea espacio (en free list)

    // cambio nodo del bloque a reservar

    // concecto nodo en ocupied

    // cambio variables generales


}

void free(void * ptr) {
    /* SEARCH of the ptr on used list */
    // creates a pointer to the real start of the block
    uint8_t * pointer = (uint8_t *)ptr - ALIGNEMENT(node);

    node * iterator = memory.usedList;
    node * prev = iterator;
    
    // search for the pointer
    while (iterator != 0 && iterator->address != pointer) {
        prev = iterator;
        iterator = iterator->next;
    }
        
    // if not found
    if (iterator == 0) return;
    node * blockToFree = iterator;

    // updates used list
    if (memory.usedList == blockToFree) memory.usedList = blockToFree->next;
    else prev->next = blockToFree->next;

    /* SEARCH found block on free list */
    iterator = memory.freeList;
    prev = iterator;

    // searches the correct position to insert
    while (iterator != 0 && iterator->address < blockToFree->address) {
        prev = iterator;
        iterator = iterator->next;
    }
    
    // inserts between node prev and iterator
    // if its the first place
    if (prev == iterator) {
        memory.freeList = blockToFree;
        blockToFree->prev = 0;
        blockToFree->next = iterator;
    } else {
        blockToFree->prev = prev;
        blockToFree->next = prev->next;
        prev->next = blockToFree;
        if (iterator != 0) iterator->prev = blockToFree; // if not last
    } 

    // updates header values
    memory.occupied -= blockToFree->size;
    memory.free += blockToFree->size;

    // checks for a next contiguous free block and merges it
    merge_next(blockToFree);
    if (blockToFree->prev != 0) merge_next(blockToFree->prev);
}

void status(uint64_t * total, uint64_t * occupied, uint64_t * free) {
    *total = memory.total * memory.pageSize;
    *occupied = memory.occupied * memory.pageSize;
    *free = memory.free * memory.pageSize;
}

void merge_next(node * block) {
    node * nextBlock = block->next;
    if (nextBlock != 0 && block->address + block->size * memory.pageSize == nextBlock->address) {
        block->size += nextBlock->size;
        block->next = nextBlock->next;
    }
}

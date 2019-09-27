// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <memoryManager.h>
#include <lib.h>

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
    node * iterator = memory.usedList;

    // creates a pointer to the real start of the block
    uint8_t * pointer = (uint8_t *)ptr - ALIGNEMENT(node);
    
    // search for the pointer
    while (iterator != 0 && iterator->address != pointer)
        iterator = iterator->next;

    // if not found
    if (iterator == 0) return;
    node * blockToFree = iterator;

    /* SEARCH found block on free list */
    iterator = memory.freeList;
    node * prev = iterator;

    // searches the correct position to insert
    while (iterator != 0 && iterator->address < blockToFree->address) {
        prev = iterator;
        iterator = iterator->next;
    }
    
    // inserts after the node prev
    blockToFree->next = prev->next;
    blockToFree->prev = prev;
    prev->next = blockToFree;

    // updates header values


    // checks for contiguous free blocks (next or prev) and joins them
    check_contiguity(blockToFree);
}

void status(uint64_t * total, uint64_t * occupied, uint64_t * free) {
    *total = memory.total * memory.pageSize;
    *occupied = memory.occupied * memory.pageSize;
    *free = memory.free * memory.pageSize;
}

void check_contiguity(node * block) {
    node * prevBlock = block->prev;
    node * nextBlock = block->next;

    if (nextBlock != 0 && block->address + block->size * memory.pageSize == nextBlock->address) {
        // unimos
    }

    if (prevBlock != 0 && prevBlock->address + prevBlock->size * memory.pageSize == block->address) {
        // unimos
    }
}

// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <memoryManager.h>
#include <lib.h>
#include <console.h>

typedef union mem_node {
    struct {
        // pointers to the next and previous memory block nodes
        union mem_node * next;
        union mem_node * prev;

        // pointer to start of the block (node)
        uint8_t * address;

        // quantity of consecutive free pages
        uint64_t size;
    } n;
    long x;
} node;


typedef struct mem_header {
    node * freeList;
    node * usedList;
 
    //  measured in pages
    uint64_t occupied;
    uint64_t free;

    // page size in bytes
    uint64_t pageSize;
} header;

static void merge_next(node * block);

// header of the memory manager
static header memory;

void create_manager(uint8_t * address, uint64_t pageSize, uint64_t maxPages) {    
    // initialize list header
    memory.free = maxPages;
    memory.occupied = 0;
    memory.freeList = (node *) address;
    memory.usedList = 0;
    memory.pageSize = pageSize;

    // create first block of maxPages pages
    node first;
    first.n.next = 0;
    first.n.prev = 0;
    first.n.address = address;
    first.n.size = maxPages;

    memcpy(address, &first, sizeof(node));
}

void * malloc(uint64_t bytes) {
    // Obtener numero de paginas solicitadas, tomando en cuenta el espacio del nodo
    uint64_t pageCount = (bytes + sizeof(node)) / memory.pageSize + 1;
    if (pageCount > memory.free) return 0; // No tengo paginas disponibles

    // Recorro nodos de freeList hasta encontrar uno con capacidad suficiente
    node * found = memory.freeList;
    while (found != 0 && found->n.size < pageCount) {
        found = found->n.next;
    }
    if (found == 0) return 0; // No tengo espacio contiguo suficiente

    // Chequeo si me sobran paginas
    if (found->n.size > pageCount) {
        // Creo nodo nuevo que apunte al comienzo del siguiente bloque, con las paginas restantes y lo conecto
        node newNode;
        newNode.n.prev = found->n.prev;
        newNode.n.next = found->n.next;
        newNode.n.address = found->n.address + pageCount * memory.pageSize;
        newNode.n.size = found->n.size - pageCount;
        memcpy(newNode.n.address, &newNode, sizeof(node));
        if (found->n.prev == 0)
            memory.freeList = (node *) newNode.n.address;
        else
            found->n.prev->n.next = (node *) newNode.n.address;
    }

    // Modifico las propiedades del nodo extraido y lo agrego a usedList
    found->n.size = pageCount;
    found->n.prev = 0;
    found->n.next = memory.usedList;
    memory.usedList = found;
    
    // Modifico las propiedades de la memoria
    memory.occupied += pageCount;
    memory.free -= pageCount;

    // Devuelvo la direccion del nodo extraido desfasada ALIGNEMENT(node)
    return (void *) (found->n.address + sizeof(node));
}

void free(void * ptr) {
    /* SEARCH of the ptr on used list */
    // creates a pointer to the real start of the block
    uint8_t * pointer = (uint8_t *)ptr - sizeof(node);

    node * iterator = memory.usedList;
    node * prev = iterator;
    
    // search for the pointer
    while (iterator != 0 && iterator->n.address != pointer) {
        prev = iterator;
        iterator = iterator->n.next;
    }
        
    // if not found
    if (iterator == 0) return;
    node * blockToFree = iterator;

    // updates used list
    if (memory.usedList == blockToFree) memory.usedList = blockToFree->n.next;
    else prev->n.next = blockToFree->n.next;

    /* SEARCH found block on free list */
    iterator = memory.freeList;
    prev = iterator;

    // searches the correct position to insert
    while (iterator != 0 && iterator->n.address < blockToFree->n.address) {
        prev = iterator;
        iterator = iterator->n.next;
    }
    
    // inserts between node prev and iterator
    // if its the first place
    if (prev == iterator) {
        memory.freeList = blockToFree;
        blockToFree->n.prev = 0;
        blockToFree->n.next = iterator;
    } else {
        blockToFree->n.prev = prev;
        blockToFree->n.next = prev->n.next;
        prev->n.next = blockToFree;
        if (iterator != 0) iterator->n.prev = blockToFree; // if not last
    } 

    // updates header values
    memory.occupied -= blockToFree->n.size;
    memory.free += blockToFree->n.size;

    // checks for a next contiguous free block and merges it
    merge_next(blockToFree);
    if (blockToFree->n.prev != 0) merge_next(blockToFree->n.prev);
}

void status(uint64_t * total, uint64_t * occupied, uint64_t * free) {
    *total = (memory.free + memory.occupied) * memory.pageSize;
    *occupied = memory.occupied * memory.pageSize;
    *free = memory.free * memory.pageSize;
}

void printStatus() {
    uint64_t total = (memory.free + memory.occupied) * memory.pageSize;
    uint64_t occupied = memory.occupied * memory.pageSize;
    uint64_t free = memory.free * memory.pageSize;

    print("\n----- Estado de la memoria -----");
    print("\nTotal Size: %d\nOcuppied Size: %d\nFree Size: %d", total, occupied, free);
}

void merge_next(node * block) {
    node * nextBlock = block->n.next;
    if (nextBlock != 0 && block->n.address + block->n.size * memory.pageSize == nextBlock->n.address) {
        block->n.size += nextBlock->n.size;
        block->n.next = nextBlock->n.next;
    }
}

void * getLastAddress (void * ptr) {
    node * aux = (node *) (ptr - sizeof(node));
    return aux->n.address + aux->n.size * memory.pageSize;
}

void printMemState() {
    node * free = memory.freeList;
    print("\nLista de frees: \n");
    while (free != 0) {
        print("- Size: %d", free->n.size);
        print("- Address: 0x");
        printHex((uint64_t)free->n.address);
        print("\t to Last: 0x");
        printHex((uint64_t)getLastAddress(free->n.address + sizeof(node)));
        print("\n");
        free = free->n.next;
    }
    print("\n---------------\n: ");

    node * used = memory.usedList;
    print("\nLista de used: \n");
    while (used != 0) {
        print("- Size: %d", used->n.size);
        print("- Address: 0x");
        printHex((uint64_t)used->n.address);
        print("\n");
        used = used->n.next;
    }
    print("\n---------------\n: ");
}
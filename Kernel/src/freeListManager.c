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
    uint64_t occupied;
    uint64_t free;

    // page size in bytes
    uint64_t pageSize;
} header;

int create_manager(uint8_t * address, uint64_t pageSize, uint64_t maxPages) {
    // check for errors
    if (address + pageSize * maxPages > MAX_DIR) return -1;   
    
    // initialize list header
    memory.total = maxPages;
    memory.free = maxPages;
    memory.occupied = 0;
    memory.freeList = (node *) address;
    memory.usedList = 0;
    memory.pageSize = pageSize;

    // create first block of maxPages pages
    node first;
    first.next = 0;
    first.prev = 0;
    first.address = address;
    first.size = maxPages;

    memcpy(address, &first, sizeof(node));

    return 0;
}

void * malloc(uint64_t bytes) {
    // Obtener numero de paginas solicitadas, tomando en cuenta el espacio del nodo
    uint64_t pageCount = (bytes + ALIGNEMENT(node)) / memory.pageSize + 1;
    if (pageCount > memory.free) return 0; // No tengo paginas disponibles

    // Recorro nodos de freeList hasta encontrar uno con capacidad suficiente
    node * found = memory.freeList;
    while (found != 0 && found->size < pageCount) {
        found = found->next;
    }
    if (found == 0) return 0; // No tengo espacio contiguo suficiente

    // Chequeo si me sobran paginas
    if (found->size > pageCount) {
        // Creo nodo nuevo que apunte al comienzo del siguiente bloque, con las paginas restantes y lo conecto
        node newNode;
        newNode.prev = found->prev;
        newNode.next = found->next;
        newNode.address = (node *) (found->address + pageCount * memory.pageSize);
        newNode.size = found->size - pageCount;
        memcpy(newNode.address, &newNode, sizeof(node));
        if (found->prev == 0)
            memory.freeList = (node *) newNode.address;
        else 
            found->prev->next = newNode.address;
    }

    // Modifico las propiedades del nodo extraido y lo agrego a usedList
    found->size = pageCount;
    found->prev = 0;
    found->next = memory.usedList;
    memory.usedList = (node *) found->address;
    
    // Modifico las propiedades de la memoria
    memory.occupied += pageCount;
    memory.free -= pageCount;

    // Devuelvo la direccion del nodo extraido desfasada ALIGNEMENT(node)
    return found->address + ALIGNEMENT(node);
}

void free(void * ptr) {

    // checkeo que este en el fucking rango

    // ORDEN N, PELOTUDO

    // recorremos y buscamos bloque con address ptr

    // recorremos freelist desde principio y comparamos posiciones de memory

    // inserto ahi en el medio

    // funcion check contiguiti
}

void status(uint64_t * total, uint64_t * occupied, uint64_t * free) {
    *total = (memory.free + memory.occupied) * memory.pageSize;
    *occupied = memory.occupied * memory.pageSize;
    *free = memory.free * memory.pageSize;
}




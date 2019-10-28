#include <stdint.h>
#include <lib.h>
#include <console.h>

#include <memoryManager.h>

typedef enum {F = 0, O, PO} nodeState;

typedef struct mem_tree_node {
        /* Pointers left and right nodes */
        struct mem_tree_node * left;
        struct mem_tree_node * right;

        /* State of the current node */
        nodeState state;

        /* Pointer to start of the block (node) */
        void * address;

        /* Level, exponent of base 2 */
        uint64_t level;   
} node;

typedef struct node_header {
    /* Measured in blocks */
    uint64_t occupied;
    uint64_t free;

    /* Block size in level (exp 2) */
    uint64_t maxLevel;
    uint64_t minLevel;
    uint64_t blockSize;

    /* Block start addres */
    void * start;
    
    /* Nodes vector start */
    node * nodes;
} head;

static head header;

/* Initializes al the data for the header */
static void init_header(uint8_t * address, uint64_t blockSize, uint64_t maxBlocks) {
    /* Staring max level, min level and block size */
    header.maxLevel = exp2(blockSize * maxBlocks) - 1;
    header.minLevel = exp2(blockSize);
    header.blockSize = blockSize;

    /* Searches for the best configuration posible */
    uint64_t realBytes = pow(2, header.maxLevel);
    uint64_t nodeSpace, maxBlocks;
    do {
        nodeSpace = blockSize * maxBlocks - realBytes;
        maxBlocks = pow(2, header.maxLevel - header.minLevel + 1) - 1;
        header.maxLevel -= 1;
        realBytes /= 2;
    } while (maxBlocks * sizeof(node) > nodeSpace);

    /*  Finally initializes remaining header data */
    header.start = (void *) address;
    header.nodes = (node *)(address + realBytes); 
    header.free = realBytes / header.blockSize;
    header.occupied = 0;
}

/* Initializes vector of nodes */
void init_nodes(uint64_t index, uint8_t * address, uint64_t level) {
    /* Only if valid level */
    if (level < header.minLevel) return;

    /* Initialize current node */
    header.nodes[index].address = (void *)address;
    header.nodes[index].level = level;
    header.nodes[index].left = header.nodes + index + 1;
    header.nodes[index].right = header.nodes + index + pow(2, level - header.minLevel + 1);
    header.nodes[index].state = F;

    /* Initialize left part of the tree */
    init_nodes(index + 1, address, level - 1);

    /* Initialize right part of the tree */
    init_nodes(index + pow(2, level - header.minLevel + 1), address + pow(2, level - 1), level - 1);
}

/* Memory Manager builder, block size must be base 2 */
void create_manager(uint8_t * address, uint64_t blockSize, uint64_t maxBlocks) {
    init_header(address, blockSize, maxBlocks);
    init_nodes(0, header.start, header.maxLevel);
}

/* Recursive function, creates children if needed, returns node direction */
static void * find_block(node * n, uint64_t level) {
    if (n->level == level) {
        if (n->state != F) return 0;
        else {
            n->state = O;
            uint64_t aux = pow(2, n->level - header.minLevel);
            header.free -= aux;
            header.occupied += aux;
            return n->address;
        }
    }
    
    /* It has a superior level but occupied */
    if (n->state == O) return 0;

    /* Superior level and free or partially occupied */
    /* If its FREE we will find place for block, so is PO */
    n->state = PO;
    void * aux = find_block(n->left, level);
    if (aux == 0) aux = find_block(n->right, level);   

    /* Updates state of the current node (at this point it was PO) */
    if (n->left->state == O && n->right->state == O) n->state = O;

    return aux;
}

/* Reserves bytes space on memory */
void * malloc(uint64_t bytes) {
    return find_block(header.nodes, max(exp2(bytes), header.minLevel));
}

/* Merges childs of the given node */
static void merge_childs(node * n) {
    n->state = PO;
    /* If childs are both free */
    if (n->left->state == F && n->right->state == F) n->state = F;
}

/* Recursive function to free a block */
static uint64_t delete_block(node * n, void * ptr) {
    if (n->address == ptr) {
        n->state = F;
        uint64_t aux = pow(2, n->level - header.minLevel);
        header.free += aux;
        header.occupied -= aux;
        return 1;
    }
    
    if (n->state == F) return 0;

    /* Recursive search on childs */
    uint64_t aux = delete_block(n->left, ptr);
    if (aux == 0) aux = delete_block(n->right, ptr);

    /* If found, update state and potentially merge */
    if (aux == 1) merge_childs(n);
    return aux;
}

/* Frees space on memory */
void free(void * ptr) {
    delete_block(header.nodes, ptr); 
}

/* Gets memory status */
void status(uint64_t * total, uint64_t * occupied, uint64_t * free) {
    *occupied = header.occupied * header.blockSize;
    *free = header.free * header.blockSize;
    *total = *occupied + *free;
}

/* Prints memory status */
void printStatus() {
    uint64_t occupied = header.occupied * header.blockSize;
    uint64_t freed = header.free * header.blockSize;
    uint64_t total = occupied + freed;

    print("\n----- Estado de la memoria -----");
    print("\nTotal Size: %d\nOcuppied Size: %d\nFree Size: %d", total, occupied, freed);
}

void printNode(node * n) {
    print("\nLevel: %d - Address: 0x", n->level);
    printHex((uint64_t) n->address);
    print(" - State: %d\n", n->state);
    printNode(n->left);
    printNode(n->right);
}


/* Prints memory state */
void printMemState() {
    print("\n----- Memory State -----\n");
    printNode(header.nodes);
}
/***************************************************
    MemoryManager.h
****************************************************/

#ifndef _memmanager_
#define _memmanager_

#include <stdint.h>

#define ALIGNEMENT(x) (sizeof(x) + 10)

/* Memory Manager builder */
void create_manager(uint8_t * address, uint64_t pageSize, uint64_t maxPages);

/* Memory Manager user functions */
void * malloc(uint64_t bytes);
void free(void * ptr);
void status(uint64_t * total, uint64_t * occupied, uint64_t * free);
void printStatus();

// Returns the first address from the next block, assuming ptr is a valid return from malloc
void * getLastAddress(void * ptr);

void printMemState();

#endif
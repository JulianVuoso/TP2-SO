/***************************************************
    MemoryManager.h
****************************************************/

#ifndef _memmanager_
#define _memmanager_

#include <stdint.h>

#define MAX_DIR     0xFFFFFFFFFFFFFFFF  // end of the memory of Pure64

#define ALIGNEMENT(x) (sizeof(x) + 10)

/* Memory Manager builder */
int create_manager(uint8_t * address, uint64_t pageSize, uint64_t maxPages);

/* Memory Manager user functions */
void * malloc(uint64_t bytes);
void free(void * ptr);
void status(uint64_t * total, uint64_t * occupied, uint64_t * free);

/* auxiliar functions */
static void check_contiguity(node * block);

#endif
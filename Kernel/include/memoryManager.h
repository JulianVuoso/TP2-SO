/***************************************************
    MemoryManager.h
****************************************************/

#ifndef _memmanager_
#define _memmanager_

#include <stdint.h>

/* Memory Manager builder */
int create_manager(uint8_t adress, uint64_t pageSize, uint64_t maxPages);

/* Memory Manager user functions */
void * malloc(uint64_t size);
void free(void * ptr);
void status(uint64_t * total, uint64_t * ocupied, uint64_t * free);

#endif
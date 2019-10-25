#ifndef LIB_H
#define LIB_H

#include <stdint.h>

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);

/* Gets cpu vendor */
char * cpu_vendor(char *result);

/* Reads port (hardware) */
char read_port(char port);

/* Writes port (hardware) */
void write_port(char port, char reg);

/* Makes an atomic swap using xchg */
void atom_swap(uint64_t * dir, uint64_t value);

/* Forces timer tick to interrupt */
uint64_t force_timer_tick();

/* Gets pow */
uint64_t pow(int base, int exponent);

#endif
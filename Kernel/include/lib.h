#ifndef LIB_H
#define LIB_H

#include <stdint.h>

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);

char * cpu_vendor(char *result);

char read_port(char port);
void write_port(char port, char reg);

void atom_swap(uint64_t * dir, uint64_t value);

uint64_t force_timer_tick();

int stringlen(char * str);
void stringcp(char * destination, char * origin);
int stringcmp(char * str1, char * str2);

uint64_t pow(int base, int exponent);

#endif
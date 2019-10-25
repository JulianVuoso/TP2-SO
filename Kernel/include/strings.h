
/***************************************************
                    Strings.h
****************************************************/

#ifndef STRINGS_H_
#define STRINGS_H_

#include <stdint.h>

/* Gets lenght of string */
int strlen(char * str);

/* Copies one string onto a destination */
void stringcp(char * destination, char * origin);

/* Compares two strings */
 stringcmp(char * str1, char * str2);

uint32_t int_to_string(uint64_t value, char * buffer, uint32_t base);

#endif /* STRINGS_H_ */

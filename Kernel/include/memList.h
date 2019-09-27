/***************************************************
    MemList.h
****************************************************/

#ifndef _memlist_
#define _memlist_

#include <stdint.h>







typedef int elementType; // Si modifico el int, modifico la funcion compare


typedef struct listCDT *listADT;
listADT newList ();
int add (listADT l, elementType elem);
void delete (listADT l, elementType elem);
int size (const listADT l);
elementType get (const listADT l, unsigned int idx);

#endif
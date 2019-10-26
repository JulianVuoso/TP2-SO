#ifndef _MODULELOADER_H_
#define _MODULELOADER_H_

#include <stdint.h>
#include <lib.h>
#include <naiveConsole.h>

void * loadModules(void * payloadStart, void ** moduleTargetAddress);

#endif
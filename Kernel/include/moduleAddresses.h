#ifndef _MODULEADDRESSES_H_
#define _MODULEADDRESSES_H_

/* Si se quiere modificar alguna de estas direcciones, cambiarla tambien en el .ld correspondiente */
/* Si se quiere agregar otro moduleAddress, consultar Create Program.txt */

static void * const sampleCodeModuleAddress = (void*)0x400000;
static void * const sampleDataModuleAddress = (void*)0x500000;

static void * const processAModuleAddress = (void*)0x1000000;
static void * const processBModuleAddress = (void*)0x1100000;
static void * const shellModuleAddress = (void*)0x1200000;
static void * const idleModuleAddress = (void*)0x1400000;

#endif
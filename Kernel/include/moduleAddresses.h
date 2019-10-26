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
static void * const sleepModuleAddress = (void*)0x1500000;
static void * const loopModuleAddress = (void*)0x1600000;
static void * const catModuleAddress = (void*)0x1700000;
static void * const wcModuleAddress = (void*)0x1800000;
static void * const filterModuleAddress = (void*)0x1900000;
static void * const phyloModuleAddress = (void*)0x1A00000;

#endif
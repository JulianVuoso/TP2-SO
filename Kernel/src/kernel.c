// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdint.h>
#include <strings.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <idtLoader.h>
#include <defs.h>
#include <videoDriver.h>
#include <console.h>
#include <timelib.h>
#include <memoryManager.h>
#include <tests.h>
#include <scheduler.h>
#include <moduleAddresses.h>
#include <process.h>
#include <fileDescriptors.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;
static const uint64_t Block = 0x1000 / 128;
static const uint64_t MaxPages = 1024;

typedef int (*EntryPoint)();

void goToUserland(){
	((EntryPoint)shellModuleAddress)();
}

void clearBSS(void * bssAddress, uint64_t bssSize)
{
	memset(bssAddress, 0, bssSize);
}

void * getStackBase()
{
	return (void*)(
		(uint64_t)&endOfKernel
		+ PageSize * 8				//The size of the stack itself, 32KiB
		- sizeof(uint64_t)			//Begin at the top of the stack
	);
}

void * initializeKernelBinary()
{
	char buffer[10];

	cpu_vendor(buffer);

	// Llena con los addresses a donde copia los modulos
	void * moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress,
		processAModuleAddress,
		processBModuleAddress,
		shellModuleAddress,
		idleModuleAddress,
		sleepModuleAddress,
		loopModuleAddress,
		catModuleAddress,
		wcModuleAddress,
		filterModuleAddress,
		phyloModuleAddress
	};

	void * endOfModules = loadModules(&endOfKernelBinary, moduleAddresses);

	clearBSS(&bss, &endOfKernel - &bss);

	void * startOfMem = (void *)(((uint8_t *) endOfModules + PageSize - (uint64_t) endOfModules % PageSize));
	create_manager(startOfMem, PageSize, MaxPages);
	initScheduler();
	initFds();
	initVideoDriver();
  	init_console();
 	
	
	load_idt();

	return getStackBase();
}

int main() {
	create(shellModuleAddress, "SHELL", FORE, 0, 0);
	while (!checkLoaded());
	// create(processAModuleAddress, "SHELL", FORE);
	// while (!checkLoaded());
	// create(processAModuleAddress, "SHELL");
	// while (!checkLoaded());
    // goToUserland();
	// testMemoryManager();
	// testContextSwitch();
	return 0;
}
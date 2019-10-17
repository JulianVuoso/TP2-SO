#include <tests.h>
#include <syscalls.h>
#include <console.h>

#include <memoryManager.h>
void testMemoryManager() {
	int i = 0;
	void * vec[10];
	clear_console();
	while (i < 5){
		printMemState();
		sleep_handler(3000);
		vec[i] = malloc(10);
		i++;
	}

	
	printMemState();
	sleep_handler(3000);
	free(vec[3]);
	printMemState();
	sleep_handler(3000);
	free(vec[1]);
	printMemState();
	sleep_handler(3000);
	free(vec[2]);
	printMemState();
}

#include <process.h>
#include <scheduler.h>

static void * const processAModuleAddress = (void*)0x1000000;
static void * const processBModuleAddress = (void*)0x1100000;

void testContextSwitch() {
	Process first = create(processAModuleAddress, "P1");

	Process second = create(processBModuleAddress, "P2");

	sleep_handler(2000);

	// setFirst(first);
	// setSecond(second);
	// setGo();
	while (1) {
		
	}
}
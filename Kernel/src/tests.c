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
#include <interrupts.h>

static void * const processAModuleAddress = (void*)0x1000000;
static void * const processBModuleAddress = (void*)0x1100000;

void testContextSwitch() {
	// _cli();
	print("Process A PID: ");
	uint64_t pidA = create(processAModuleAddress, "P1", FORE,0,0);
	printHex(pidA);
	print("\nProcess B PID: ");
	uint64_t pidB = create(processBModuleAddress, "P2", FORE,0,0);
	printHex(pidB);

	listAll();
	setPriority(2, 0);
	listAll();
	
	// _sti();
	// sleep_handler(10000);
	// while (1) {
	// 	sleep_handler(5000);
	// 	int ret = kill(pidA);
	// 	print("\nRET: %d\n", ret);
	// }
}
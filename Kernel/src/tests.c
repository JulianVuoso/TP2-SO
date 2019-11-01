// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdint.h>
#include <syscalls.h>
#include <console.h>
#include <memoryManager.h>

#include <tests.h>

void testMemoryManager() {
	int i = 0;
	void * vec[10];
	clear_console();
	while (i < 5){
		mm_print_state();
		// sleep_handler(3000);
		vec[i] = malloc(10);
		print("\n\tMalloqueo: 0x");
		printHex((uint64_t) vec[i]);
		i++;
	}

	
	mm_print_state();
	// sleep_handler(3000);
	free(vec[3]);
	mm_print_state();
	// sleep_handler(3000);
	free(vec[1]);
	mm_print_state();
	// sleep_handler(3000);
	free(vec[2]);
	mm_print_state();
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
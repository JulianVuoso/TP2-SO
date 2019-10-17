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

void p1 () {
	while (1) {
		print("\n----------- p1 -----------\n");
		sleep_handler(55);
	}
}

void p2 () {
	while (1) {
		print("\n########### p2 ###########\n");
		sleep_handler(55);
	}
}

#include <process.h>
#include <scheduler.h>
void testContextSwitch() {
	printHex((uint64_t)&p1);
	Process first = createProcess(&p1, "P1");
	printHex((uint64_t)&p2);
	Process second = createProcess(&p2, "P2");

	sleep_handler(2000);

	setFirst(first);
	setSecond(second);
	setGo();
}
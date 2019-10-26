// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <syscalls.h>
#include <stdint.h>

#define SYSCALL_COUNT	18


// Software handlers functions
static uint64_t syscall_00 (uint64_t rdi, uint64_t rsi, uint64_t rdx);
static uint64_t syscall_01 (uint64_t rdi, uint64_t rsi, uint64_t rdx);
static uint64_t syscall_02 (uint64_t rdi, uint64_t rsi, uint64_t rdx);
static uint64_t syscall_03 (uint64_t rdi, uint64_t rsi, uint64_t rdx);
static uint64_t syscall_04 (uint64_t rdi, uint64_t rsi, uint64_t rdx);
static uint64_t syscall_05 (uint64_t rdi, uint64_t rsi, uint64_t rdx);
static uint64_t syscall_06 (uint64_t rdi, uint64_t rsi, uint64_t rdx);
static uint64_t syscall_07 (uint64_t rdi, uint64_t rsi, uint64_t rdx);
static uint64_t syscall_08 (uint64_t rdi, uint64_t rsi, uint64_t rdx);

static uint64_t syscall_09 (uint64_t rdi, uint64_t rsi, uint64_t rdx);
static uint64_t syscall_10 (uint64_t rdi, uint64_t rsi, uint64_t rdx);
static uint64_t syscall_11 (uint64_t rdi, uint64_t rsi, uint64_t rdx);

static uint64_t syscall_12 (uint64_t rdi, uint64_t rsi, uint64_t rdx);
static uint64_t syscall_13 (uint64_t rdi, uint64_t rsi, uint64_t rdx);
static uint64_t syscall_14 (uint64_t rdi, uint64_t rsi, uint64_t rdx);
static uint64_t syscall_15 (uint64_t rdi, uint64_t rsi, uint64_t rdx);
static uint64_t syscall_16 (uint64_t rdi, uint64_t rsi, uint64_t rdx);
static uint64_t syscall_17 (uint64_t rdi, uint64_t rsi, uint64_t rdx);
static uint64_t syscall_18 (uint64_t rdi, uint64_t rsi, uint64_t rdx);

uint64_t (* syscalls[]) (uint64_t rdi, uint64_t rsi, uint64_t rdx) = {syscall_00, syscall_01, syscall_02, syscall_03, 
																	syscall_04, syscall_05, syscall_06, syscall_07,
																	syscall_08,	syscall_09, syscall_10, syscall_11,
																	syscall_12, syscall_13, syscall_14, syscall_15,
																	syscall_16, syscall_17, syscall_18};

// Dispatcher for software interrupts
uint64_t handleSyscall(uint64_t sirq, uint64_t rdi, uint64_t rsi, uint64_t rdx) {
	if (sirq <= SYSCALL_COUNT) {
		return syscalls[sirq](rdi, rsi, rdx); 
	}
	return 1;
}

uint64_t syscall_00 (uint64_t rdi, uint64_t rsi, uint64_t rdx) {
	read_handler(rdi, (char *)rsi, rdx);
	return 0;
}

uint64_t syscall_01 (uint64_t rdi, uint64_t rsi, uint64_t rdx) {
	write_handler(rdi, (char *)rsi, rdx);
	return 0;
}

uint64_t syscall_02 (uint64_t rdi, uint64_t rsi, uint64_t rdx) {
	return time_handler();
}

uint64_t syscall_03 (uint64_t rdi, uint64_t rsi, uint64_t rdx) {
	clear_handler();
	return 0;
}

uint64_t syscall_04 (uint64_t rdi, uint64_t rsi, uint64_t rdx) {
	return rtc_handler((char) rdi);
}

uint64_t syscall_05 (uint64_t rdi, uint64_t rsi, uint64_t rdx) {
	sleep_handler(rdi);
	return 0;
}

uint64_t syscall_06 (uint64_t rdi, uint64_t rsi, uint64_t rdx) {
	beep_handler(rdi, rsi);
	return 0;
}

uint64_t syscall_07 (uint64_t rdi, uint64_t rsi, uint64_t rdx) {
	exit_handler();
	return 0;
}

uint64_t syscall_08 (uint64_t rdi, uint64_t rsi, uint64_t rdx) {
	pixel_handler(rdi, rsi, rdx);
	return 0;
}

/* -----------------------------------------------------------------*/
uint64_t syscall_09 (uint64_t rdi, uint64_t rsi, uint64_t rdx) {
	return (uint64_t) malloc_handler(rdi);
}

uint64_t syscall_10 (uint64_t rdi, uint64_t rsi, uint64_t rdx) {
	free_handler((void *) rdi);
	return 0;
}

uint64_t syscall_11 (uint64_t rdi, uint64_t rsi, uint64_t rdx) {
	printStatus_handler();
	return 0;
}

/* -----------------------------------------------------------------*/
uint64_t syscall_12 (uint64_t rdi, uint64_t rsi, uint64_t rdx) {
	return create_handler((void *)rdi, (char *)rsi, (level)rdx);
}

uint64_t syscall_13 (uint64_t rdi, uint64_t rsi, uint64_t rdx) {
	return kill_handler(rdi);
}

uint64_t syscall_14 (uint64_t rdi, uint64_t rsi, uint64_t rdx) {
	return getPid_handler();
}

uint64_t syscall_15 (uint64_t rdi, uint64_t rsi, uint64_t rdx) {
	listAllProcess_handler();
	return 0;
}

// rdi = PID ; rsi = PRIORITY
uint64_t syscall_16 (uint64_t rdi, uint64_t rsi, uint64_t rdx) {
	return setPriority_handler(rdi, rsi);
}

// rdi = PID
uint64_t syscall_17 (uint64_t rdi, uint64_t rsi, uint64_t rdx) {
	return changeState_handler(rdi);
}

uint64_t syscall_18 (uint64_t rdi, uint64_t rsi, uint64_t rdx) {
	halt_handler();
	return 0;
}
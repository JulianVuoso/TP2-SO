#ifndef SYSCALLS_H_
#define SYSCALLS_H_

#include <stdint.h>
#include <lib.h>
#include <process.h>

void read_handler(uint64_t fd, char * buff, uint64_t count);
void write_handler(uint64_t fd, const char * buff, uint64_t count);
uint64_t time_handler();
void clear_handler();
uint64_t rtc_handler(uint8_t register);
void sleep_handler(uint64_t millis);
void beep_handler(uint16_t frequency, uint64_t state);
void exit_handler();
void pixel_handler(uint64_t x, uint64_t y, uint64_t rgb);

void * malloc_handler(uint64_t bytes);
void free_handler(void * ptr);
void printStatus_handler();
uint64_t create_handler(void * entryPoint, char * name, level context);
uint64_t kill_handler(uint64_t pid);
uint64_t getPid_handler();
void listAllProcess_handler();
uint64_t setPriority_handler(uint64_t pid, uint8_t prio);
uint64_t changeState_handler(uint64_t pid);
void halt_handler();

uint64_t handleSyscall(uint64_t sirq, uint64_t rdi, uint64_t rsi, uint64_t rdx);

#endif
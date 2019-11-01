// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <process.h>
#include <stdint.h>
#include <lib.h>
#include <keyboard.h>
#include <timelib.h>
#include <console.h>
#include <naiveConsole.h>
#include <sound.h>
#include <memoryManager.h>
#include <scheduler.h>
#include <interrupts.h>
#include <moduleAddresses.h>
#include <fileDescriptors.h>
#include <mutex.h>

#include <syscalls.h>

extern void hang(); // Ubicada en loader.asm
static void * getModuleAddress(char * name);

void read_handler(uint64_t fd, char * buff, uint64_t count) {
    read(fd, buff, count);
}

void write_handler(uint64_t fd, const char * buff, uint64_t count) {
    write(fd, buff, count);
}

uint64_t time_handler() {
    return ticks_elapsed();
}

void clear_handler() {
    clear_console();
	// ncClear();
}

uint64_t rtc_handler(uint8_t reg){
    write_port(0x70, reg);
    uint64_t aux = read_port(0x71);
    return (aux >> 4) * 10 + (aux & 0x0F);
}

void sleep_handler(uint64_t millis){
    sleep(millis);
}

void beep_handler(uint16_t frequency, uint64_t state) {
    switch (state)
    {
        case 1: // PLAY
            play_sound(frequency);
            break;
        case 0: // STOP
            no_sound();
            break;
        default:
            break;
    }
}

void exit_handler() {
    hang();
}

void pixel_handler(uint64_t x, uint64_t y, uint64_t rgb) {
    Vector2 auxPos = {x, y};
    Color auxColor = {(rgb & 0xFF0000) >> 16, (rgb & 0x00FF00) >> 8, rgb & 0x0000FF};
    draw_pixel(auxPos, auxColor);
}

/* ---------------------------- */

void * malloc_handler(uint64_t bytes) {
    return malloc(bytes);
}

void free_handler(void * ptr) {
    free(ptr);
}

void printStatus_handler() {
    mm_print_status();
}

void mm_get_status_handler(uint64_t * total, uint64_t * occupied, uint64_t * free) {
    mm_status(total, occupied, free);
}

static int strcpyUntilSpace (char *dest, const char *src) {
	int count = 0;
	while (*src != 0 && *src != ' ') {
		*dest = *src;
		dest++;
		src++;
		count++;
	};
	*dest = 0;
	return count;
}

static int isNumber(char character){
	if (character >= '0' && character <= '9')
		return 1;
	return 0;
}

static int atoi(char* buffer, int * cursor) {
	int i = 0;
	int result = 0;
	int len = strlen(buffer);

	while(buffer[i] != 0 && isNumber(buffer[i])){
		result += (pow(10, --len) * (buffer[i] - '0'));
		i++;
	}
    *cursor = i;
	if (buffer[i] == 0 && i != 0)
		return result;
	return -1;
}


//syscall(NEW_PROC_ID, (uint64_t) name, argc, (uint64_t) argv, ground, inFd, outFd)
// uint64_t create_handler(void * entryPoint, char * name, level context) {
uint64_t create_handler(char * name, uint64_t argc, char ** argv, level context, char * fds) {
    int inFd = 0, outFd = 1, cursor = 0;
    char aux[5];

    strcpyUntilSpace(aux, fds);
    inFd = atoi(aux, &cursor);
    strcpyUntilSpace(aux, fds + cursor + 1);
    outFd = atoi(aux, &cursor);

    print("\n\t\t\tIN: %d, OUT: %d", inFd, outFd);

    void * entryPoint = getModuleAddress(name);
    if (entryPoint == 0 || inFd < 0 || outFd < 0) // NOT FOUND
        return 0;
    return create(entryPoint, name, context, inFd, outFd);
}

static int strcmp (const char *t, const char *s) {
	while (*t==*s && *t!=0){
		t++;
		s++;
	}
	return *t-*s;
}

static void * getModuleAddress(char * name) {
    if (strcmp(name, "SHELL") == 0)
        return shellModuleAddress;
    if (strcmp(name, "SLEEP") == 0)
        return sleepModuleAddress;
    if (strcmp(name, "LOOP") == 0)
        return loopModuleAddress;
    if (strcmp(name, "CAT") == 0)
        return catModuleAddress;
    if (strcmp(name, "WC") == 0)
        return wcModuleAddress;
    if (strcmp(name, "FILTER") == 0)
        return filterModuleAddress;
    if (strcmp(name, "PHYLO") == 0)
        return phyloModuleAddress;
    return 0;
}

uint64_t kill_handler(uint64_t pid) {
    return kill(pid);
}

uint64_t getPid_handler() {
    return getPid();
}

void listAllProcess_handler() {
    listAll();
}

uint64_t setPriority_handler(uint64_t pid, uint8_t prio) {
    return setPriority(pid, prio);
}

uint64_t changeState_handler(uint64_t pid) {
    states state = getState(pid);
	switch (state) {
		case READY:
			return setState(pid, BLOCKED);
		case BLOCKED:
			// DEBERIA HACER ALGO MAS ACA (POR EG VER QUE PUEDA DESBLOQUEARSE)
			return setState(pid, READY);
		default: // RUNNING o UNDEFINED
			return 1;
	}
}

void halt_handler() {
    _hlt();
}

int new_pipe_handler(char * name) {
    return newPipe(name);
}

int pipe_open_handler(char * name) {
    return openPipe(name);
}

void pipe_close_handler(int fd) {
    closePipe(fd);
}

void pipe_status_handler(){
    showAllPipes();
}

uint64_t new_sem_handler(char * name, uint64_t init){
    return (uint64_t) newSem(name, init);
}

uint64_t sem_open_handler(char * name) {
    return (uint64_t) openSem(name);
}

void sem_close_handler(SemNode * sem){
    closeSem(sem);
}

void sem_wait_handler(SemNode * sem){
    waitSem(sem);
}

void sem_post_handler(SemNode * sem){
    postSem(sem);
}

void sem_status_handler(){
    showAllSem();
}

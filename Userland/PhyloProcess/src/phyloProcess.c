// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <clib.h>
#include <phyloProcess.h>

void sit();

int main() {
	while(1){
		sleep(2000);
		sit();
		sleep(2000);
	}
	return 0;

	wait(getPid());
	// antes de hacer nada que involucre sus semaforos por si no lo cree aun
} 

void sit(){

}
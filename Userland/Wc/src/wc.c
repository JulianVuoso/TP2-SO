// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/* sampleCodeModule.c */
#include <clib.h>
int main() {
	int car;
	uint64_t lines = 0;
	while ((car = getchar()) >= 0) {
		if (car == '\n')
			lines++;
	}
	printf("\n-- Cantidad de Lineas ingresadas: %d --\n", lines);
	return 0;
} 
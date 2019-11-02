// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <phyloView.h>

int main() {
	uint64_t mutex = semOpen("buffer");
	while(1){
		sleep(5000);
		for(int i = 0; i < ph_qty ; i++){
			puts("Phylo number ");
			putchar(i);
			puts(": ");
			semWait(mutex);
			puts(buffer[i]==0?"WORKING":(buffer[i]==1?"WAITING":"EATING"));
			semPost(mutex);
		}		
	}
	return 0;
} 
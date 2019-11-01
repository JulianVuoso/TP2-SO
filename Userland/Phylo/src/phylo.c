// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <phylo.h>

typedef enum {NONE = 0, WAIT, EAT} ph_state;

ph_qty = INIT_PH;

int state[];
int pids[];

int init = 0;

//SemNode * table, filo, mutex; 

int main() {
	puts("\n\n-- Programa de Filosofos --\n");
	
	char in=0;

	if(!init)
		initialize();

	while ((in=getchar()) != 'q' || in!='a'|| in!='r'){
		switch(in){
			case 'a': addPhylo(); break;
			case 'r': removePhylo(); break;
			case 'q': quitPhylo(); break;
			default: break;
		}
	}
	return 0;
} 

static void * const pMA = (void*)0x1500000;	// Cambiar numero

void initialize(){
	
	char* name;
	int pid;
	for(int p=0; p<INIT_PH ; p++, name=PH_NAME){
		concat(name, p);
		pid = create(pMA /* ni idea */, name, FORE, 0, 0);
		newSem(pid, 0);
		//wait(pid);
		pids[p] = pid;
		concat(name=PH_NAME, pid);
		newSem(name,1);
		post(pid);
	}
	newSem("buffer", 1);
	newSem(SEM_TABLE, 1);

	return;
}

void addPhylo(){
	ph_qty++;
	char*name=PH_NAME;
	concat(name,ph_qty);
	pids[ph_qty]=create(pMA/* ni idea */, name, FORE, 0,0);
	concat(name=PH_NAME, pids[ph_qty]);
	newSem(name,1);

	return;
}

void removePhylo(){
	kill(pids[ph_qty]);
	char * name = SEM_PHYLO;
	concat(name,pids[ph_qty--]);
	closeSem(name);
	return;
}
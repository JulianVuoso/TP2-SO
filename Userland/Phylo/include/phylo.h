/***************************************************
                    Phylo.h
****************************************************/

#include <clib.h>
#include <mutex.h>
#include <utils.h>
#include <process.h>

#define INIT_PH 5
#define INIT_FORKS 5
#define MAX_PHYLO 15
#define SEM_TABLE "table"
#define SEM_PHYLO "phyloSem"
#define PH_NAME "phyloNumber"

char * buffer[];

int ph_qty;

/* Start Phylo process: create philosophers and semaphores */
void initialize();

/* Create one semaphore for each philosopher */
void phylo_sems(int cant);

/* Add new philosopher */
void addPhylo();

/* Remove philosopher */
void removePhylo();

/* Exit game */
void quitPhylo();

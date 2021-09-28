#include "structures.h"
#include "../mpx_supt.h"
#include <string.h>


// constants to be used throughout functions
#define READY 0
#define BLOCKED 1

#define NOT_SUSPENDED 0
#define SUSPENDED 1

#define SYSTEM 0
#define APPLICATION 1

void swap(char *a, char *b);
char* flip(char *str, int length);
char* itoa(int number, char *str );

/*
FindPCB() is given a name of a PCB. Searches all queues until it finds a match.
When it does, returns a pointer to the PCB of the same name. If no match is found, return NULL
*/
pcb* findPCB(char* name);

pcb* allocatePCB();

pcb* setupPCB(char* newName, unsigned char newClass, int newPriority);

char* freePCB( pcb* name );

void insertPCB( pcb* pntr);

int removePCB (pcb* pntr);

void showPCB(pcb* ptr);

void showqueue(struct queue allpcbs);
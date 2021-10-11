#include "../mpx_supt.h"
#include <string.h>


//Structures

struct context {
	u32int gs, fs, es, ds;
	u32int edi, esi, ebp, esp, ebx, edx, ecx, eax;
	u32int eip, cs, eflags;
};

struct PCB {
	
	char name[20];			// array/name of PCB
	unsigned char class;	// class (digit/letter indicating whether system or app) of PCB
	int prio;				// priority level (0-9) of PCB
	int state;				// current state (ready/running/blocked) of PCB
	int susp;				// suspend (suspended/not suspended) state of PCB
	unsigned char stack[1024];		// the stack (contains all info process needs) of the PCB
	unsigned char *stackTop;// pointer to top of stack
	unsigned char *stackBase;	// pointer to bottom of stack
	struct PCB* next;						// pointer to next PCB/linked list
	struct PCB* prev;						// pointer to previous PCB/linked list				
};
typedef struct PCB pcb;						// shortcut name of "struct PCB" to be "pcb"

struct queue{
	int count; 
	pcb *head; // dequeue this 
	pcb *tail; // enquue affter this 
};


// constants to be used throughout functions
#define READY 0
#define BLOCKED 1
#define RUNNING 2

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

void showqueue(char* queue);

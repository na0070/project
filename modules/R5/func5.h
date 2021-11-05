#include "../mpx_supt.h"
#include <mem/heap.h>
#include <string.h>
#include <core/serial.h>
#include "../R2/func.h"



#define FREE 0
#define ALLOCATED 1

// for boolean statements
#define TRUE 1
#define FALSE 0

// CMCB and LMCB structs
typedef struct cmcb{
 int type; 
 u32int address;
 int size;
 //char *name;  
 struct cmcb *next;
 struct cmcb *prev;
}  CMCB;

typedef struct lmcb{
 int type; 
 int size;
}  LMCB;


struct list {
        CMCB *head; 
	CMCB *tail;  
};

// function prototypes

u32int* init_heap(int size);

u32int allocateMemory(int size);

void freeMemory (u32int address);

int isEmpty();

void showList(int printType);






//Structures

//A PCB must contain name,class,prio,state,stack,pointer to next PCB

struct PCB {
	
	char name[30];			// array/name of PCB
	unsigned char class;	// class (digit/letter indicating whether system or app) of PCB
	int prio;				// priority level (0-9) of PCB
	int state;				// current state (ready/running/blocked) of PCB
	int susp;				// suspend (suspended/not suspended) state of PCB
	unsigned char stack[1024];		// the stack (contains all info process needs) of the PCB
	// unsigned char *stackTop = stack[0];// pointer to top of stack
	// unsigned char *stackBot = stack[1023];	// pointer to bottom of stack
	struct PCB* next;						// pointer to next PCB/linked list
};
typedef struct PCB pcb;						// shortcut name of "struct PCB" to be "pcb"

struct queue{
int count; 
pcb *head; // dequeue this 
pcb *tail; // enquue affter this this 
};

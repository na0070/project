



//Structures

//A PCB must contain name,class,prio,state,stack,pointer to next PCB

struct PCB {
	
	char name[30];			// name of PCB
	unsigned char class;	// class pf PCB
	int prio;				// Priority level of PCB
	int state;				// Current state of PCB
	int susp;				// suspend state of PCB
	unsigned char stack[1024];		// The stack of the PCB
	// unsigned char *stackTop = stack[0];// pointer to top of stack
	// unsigned char *stackBot = stack[1023];	// pointer to bottom of stack
	struct PCB* next;						// pointer to next PCB
};
typedef struct PCB pcb;						// shortcut name of "struct PCB" to be "pcb"

struct queue{
int count; 
pcb *head; // dequeue this 
pcb *tail; // enquue affter this this 
};

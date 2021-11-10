
// where all the functions of R2 would be (along with itoa())
//also where all the queues are initialized for function use

#include "func.h"


struct queue ready = {.head = NULL, .tail = NULL};					// initializing the ready queue


struct queue blocked = {.head = NULL, .tail = NULL};				// initializing the blocked queue


struct queue sus_ready = {.head = NULL, .tail = NULL};				// initializing the suspended ready queue

struct queue sus_blocked = {.head = NULL, .tail = NULL};			// initializing the suspended blocked queue

struct queue* returnQueue() {

	return &ready;
}
/*
FindPCB() is given a name of a PCB. Searches all queues until it finds a match.
When it does, returns a pointer to the PCB of the same name. If no match is found, return NULL
*/
pcb* findPCB(char* name) {
	
	pcb* ptr = ready.head;					// the indexing pointer (used for traversing each queue, starting with ready queue)
	
	while (ptr != NULL) {					// loop through ready queue
		
		if (strcmp(name, ptr->name) == 0)	// if given name matches current PCB's name
			return ptr;						// return the pointer (i.e PCB found)
		
		ptr = ptr->next;					// go to next PCB in queue
	}
	
	// not found in ready, try in blocked
	
	ptr = blocked.head;						
	
	while (ptr != NULL) {					// loop through blocked queue
		
		if (strcmp(name, ptr->name) == 0) 	// if given name matches current PCB's name
			return ptr;						// return the pointer (i.e PCB found)
		
		ptr = ptr->next;					// go to next PCB in queue
	}
	
	// not found in blocked, try in suspended ready
	
	ptr = sus_ready.head;				// the indexing pointer
	
	while (ptr != NULL) {					// loop through sus_ready queue

		if (strcmp(name, ptr->name) == 0) 	// if given name matches current PCB's name
			return ptr;						// return the pointer (i.e PCB found)
		
		ptr = ptr->next;					// go to next PCB in queue
	}
	
	// not found in suspended ready, try in suspended blocked
	
	ptr = sus_blocked.head;				// the indexing pointer
	
	while (ptr != NULL) {					// loop through sus_ready queue
	
		if (strcmp(name, ptr->name) == 0) 	// if given name matches current PCB's name
			return ptr;						// return the pointer (i.e PCB found)
		
		ptr = ptr->next;					// go to next PCB in queue
	}

	// not found in any queue, then PCB doesn't exist, return null
	return NULL;
}


pcb* allocatePCB() {
	pcb* alloc = (pcb*)sys_alloc_mem(sizeof(pcb)); // initialized a pcb pointer, allowing pointer to point at a specific location of memory (memory is the size of pcb)
	alloc->prio=5;
	alloc->susp=0;
	return alloc;
}


	// setupPCB takes 3 parameters: name, class and priority of the new PCB to be created. Either returns the new pcb pointer if successful, or NULL if there is an error
	// perform error checking first (are given values valid?)
pcb* setupPCB(char* newName, unsigned char newClass, int newPriority) {	
	
	if (strlen(newName) > 20)		// is the given name larger than 20 characters? (max allowed is 20)
		return NULL;
	
	if (newClass != SYSTEM && newClass != APPLICATION)			// is the given class not "system" or "application"? (we defined system as 0 and application as 1)
		return NULL;
	
	if (newPriority < 0 || newPriority > 9)		// is the given priority not between 0-9? (anything outside that range is invalid)
		return NULL;
	
	// if we reached this line, then there are no errors
	
	pcb* newPCB = allocatePCB();									// use allocatePCB() to create space for a new PCB and assign it to the pointer "newPCB"
	
	strcpy(newPCB->name, newName);									// store the given name to the newPCB's name (string copy)
	newPCB->class = newClass;										// store the given class to the newPCB's class
	newPCB->prio = newPriority;										// store the given priority to the newPCB's priority
	
	newPCB->state = READY;											// initially set the PCB's state to ready
	newPCB->susp = NOT_SUSPENDED;									// initially set the PCB's status to not suspended

	newPCB->stackBase = newPCB->stack;
	newPCB->stackTop = newPCB->stackBase + 1024 - sizeof(struct context);


	return newPCB;													// after all the initialization is complete, return the new pcb pointer to wherever this function was used
}



char* freePCB( pcb* name ){

return (char*)sys_free_mem(name );// return free memmory function
}// free pcb end 



void insertPCB( pcb* pntr) {

	if (findPCB(pntr->name)  == NULL) {									// checking if PCB exist 

		int state = pntr->state; 										// taking the pointer state 
			int priorty = pntr->prio; 									// taking the pointer priorty 
				int sus = pntr->susp;									// taking the pointer suspend status 
				
				struct queue* Q;										// Queue pointer to indicate which queue to look in
				
				if ( state == READY){									// if pcb's state is "ready"
					
						
					if (sus == SUSPENDED)								// if pcb's status is "suspended"
						Q = &sus_ready;								 	// will look in "suspended ready"
					else
						Q = &ready;										// otherwise, will look in "not suspended ready"
					
					
						pcb* Qpntr = Q->head;							// create a pointer "Qpntr" to be used for traversing the queue
						
						if (Q->head == NULL) {							// if queue is empty
							Q->head = pntr;								// set the head and tail of the queue to this pcb
							Q->tail = pntr;

							pntr->next = NULL;							// make sure the pcb does not point to any other pcb
							pntr->prev = NULL;
							
							
						}
							else {										// queue is not empty, go to next scenario
								
								if(priorty > Q->head->prio) {			// trying to replace queue's head with new pcb (when new priority is strictly greater than head's priority)
											
										pntr->next = Q->head;			// make the new pcb point to the head as its "next"
										pntr->prev = NULL;				// make the new pcb point to nothing as its "previous"
										
										Q->head->prev = pntr;			// make the old head's previous point to the new pcb (indicating its no longer the first in queue)
										Q->head = pntr;					// assign the new pcb as the new head of the queue
											
										}	
						
								else									// not touching the head, then either inserting in the middle or end of queue
								while (Qpntr != NULL) {					// while Qpntr is still traversing through
									
									if (priorty <= Qpntr->prio){		// if new pcb's priority is less or equal to Qpntr's priority (will need to traverse to next pcb in queue)
										
										if(Qpntr->next == NULL) {		// if there *isn't* another pcb after where Qpntr is at (i.e Qpntr is currently the last one in the queue)
											
											Qpntr->next = pntr;			// make the pcb at the end of the queue point to the new pcb as its "next" (since new pcb will become last in line)
											Q->tail = pntr;				// indicate the new pcb as the new tail of the queue
											pntr->prev = Qpntr;			// make the new pcb point to the old tail as its "previous"
											pntr->next = NULL;			// make sure the new pcb doesn't point to anything else as its "next" (since its the last in line)
											break;						// exit from the loop since we are done with inserting
											
											
										}	// above block is skipped if Qpntr is not point to the end of the queue (there are more pcbs to traverse)
										
										Qpntr = Qpntr->next;			// make Qpntr point to the next in line
										
										
									}
									else  {								// if new pcb's priority is greater than Qpntr's priority (i.e we want to insert new pcb before Qpntr's pcb)
										
										pntr->next = Qpntr;				// make the new pcb point to Qpntr as its "next"
										pntr->prev = Qpntr->prev;		// make the new pcb point to Qpntr's previous as its own "previous"
										
										Qpntr->prev->next = pntr;		// make that previous pcb point to the new pcb as its "next"
										Qpntr->prev = pntr;				// now make Qpntr's previous become the new pcb (and complete all connections)
										break;							// exit the loop since inserting is complete
									}
									
								}
							}
					// if state is blocked, do this instead
				} else {									// in blocked (FIFO)
				
				
						if (sus == SUSPENDED)				// again, check if new pcb is suspended or not suspended and choose correct queue to look in
							Q = &sus_blocked;
						else
							Q = &blocked;
						
						if (Q->head == NULL) {				// if the queue is empty (will need to assign the new pcb as the head)
							
							Q->head = pntr;					// make the new pcb as the head and tail of the queue
							Q->tail = pntr;
							pntr->next = NULL;				// make sure the new pcb is not pointing to anything
							pntr->prev = NULL;
							
						}
						else {								// if queue is not empty
							
							pntr->prev = Q->tail;			// since doing FIFO, then will simply add the new pcb to the end of the queue (making the new pcb point to the tail as "previous")
							pntr->prev->next = pntr;		// make that tail point to the new pcb as "next"
							Q->tail = pntr;					// set the new pcb as the new tail of the queue
							pntr->next = NULL;				// make sure the new pcb does not point to anything next
							
						}
						
						
				}	// outside of blocked inserting
				
				Q->count = Q->count+1;						// increment the size of the queue after insertion
	} 

	else													// if pcb's name was already found existing in any queue					
		print("ERROR: PCB already exists.",27);
	
}

int removePCB(pcb* pntr) {
	
	int susp = pntr->susp;									// store pcb's status
	int state = pntr->state;								// store pcb's state
	
	struct queue* Q;										// set a queue pointer to indicate which queue to remove from
	
	
	if (pntr == NULL) {										// if the pcb given is NULL 
		
		return -1; 												// Error code
		
	} else {
		
		if (state == READY)									// check what state and status does the pcb have to choose correct queue
			if (susp == SUSPENDED)
				Q = &sus_ready;								
			else
				Q = &ready;
		else
			if (susp == SUSPENDED)
				Q = &sus_blocked;
			else
				Q = &blocked;
		

		// trying to remove head
		if (pntr == Q->head) {								// if the pcb is the head of the queue it is in
			Q->head = pntr->next;							// make the head become the next pcb in line
			pntr->next->prev = NULL;						// make that new head point to nothing as "previous"
			pntr->next = NULL;								// make the removed pcb point to nothing next
			Q->count = Q->count-1;							// decrease the size of the queue and return a success code
			return 0;
		}
		
		//trying to remove tail
		if (pntr == Q->tail) {								// if the pcb is the tail of the queue
			
			Q->tail = pntr->prev;							// make the previous pcb as the new tail 
			pntr->prev->next = NULL;						// make that new tail not point to anything next

			pntr->prev = NULL;								// make the removed pcb point to nothing else
			pntr->next = NULL;

			Q->count = Q->count-1;							// decrement the size of the queue and return success code
			return 0;
			
		}
		
	// removing from the middle
		pntr->prev->next = pntr->next;            // instead of A pointing to B, A pointing to C
			pntr->next->prev = pntr->prev; 		// instead of C pointing to B, C pointing to A
			
			pntr->next = NULL; 							// unlink next pointer of B "C"
		pntr->prev = NULL;								// unlink previous pointer of B "A"
		
		Q->count = Q->count-1;
		
	}  return 0; 												// success
			
}



// showPCB prints information for a single pcb (using the pointer)
void showPCB(pcb* ptr) {
	

	print("\nName: ",8);
	print(ptr->name,20);

	print("\nClass: ",9);
	if (ptr->class == SYSTEM)													// print the class (based on value)

		print("SYSTEM",7);
	
	else

		print("APPLICATION",12);

	print("\nState: ",9);
	
	if (ptr->state == READY)													// print the state (based on value)
		print("READY",7);
	
	else
		print("BLOCKED",8);


	print("\nStatus: ",10);
	
	if (ptr->susp == NOT_SUSPENDED)													// print the status (based on value)
		print("NOT SUSPENDED",14);
		
	else
		print("SUSPENDED",10);
	

	print("\nPriority: ",12);
	
	char prioNum[1];
	itoa(ptr->prio,prioNum);
							
	print(prioNum,strlen(prioNum));				// print the priority (based on value)
	
}
void showqueue(char* queue){						 //function used to print an entire queue using the function showpcb()

	pcb* ptr;
	if(strcmp(queue,"ready") == 0)					//set a pcb pointer to the head of a given queue
		ptr=ready.head; 
	else if(strcmp(queue,"sus_ready") == 0) 
		ptr=sus_ready.head;
	else if(strcmp(queue,"blocked") == 0) 
		ptr=blocked.head; 
	else
		ptr=sus_blocked.head;
	
	while(ptr!=NULL){										 //while still not reaching end of the queue
		showPCB(ptr); 										//print the current pcb's info
		print("\n***",5);
		ptr=ptr->next;									 	//go to next pcb in queue
		
	}
}

void loadr_pcb(char* name, unsigned char class,int status, int priority, u32int func ){

	if (findPCB(name) != NULL)							// if a copy of a process is already in memory, don't load a new one
		print("process already exists\n",24);

	else {

   	pcb * new_pcb = setupPCB ( name , class , priority );
   	new_pcb -> state = READY;
   	new_pcb -> susp = status;

    	context * cp = ( context *)( new_pcb -> stackTop );
     	memset ( cp , 0, sizeof ( context ));
      cp -> fs = 0x10 ; 
      cp -> gs = 0x10 ;
      cp -> ds = 0x10 ;
      cp -> es = 0x10 ;
      cp -> cs = 0x8 ;
      cp -> ebp = ( u32int )( new_pcb -> stack );
      cp -> esp = ( u32int )( new_pcb -> stackTop ); 
      cp -> eip = ( u32int ) func ;// The function correlating to the process , ie. Proc1
      cp -> eflags = 0x202 ;
      insertPCB(new_pcb);
   }
}



//==============================================================================================================


// itoa() and helper functions below

// used to swap two given values with each other (useful for arrays) using pointers to chars
void swap(char *a, char *b) {
	
	char temp = *a; 	// temporarily store first value to avoid losing it
	*a = *b;			// store the second value in the first position
	*b = temp;			// store the first value in the seocnd position
}


// used to flip a given string array (first charatcter becomes last, etc) using swap() finction in a loop
// takes the string to be flipped, and the string's length
char* flip(char *str, int length) {
	
	int i = 0, j = length-1;	// two indices at beginning and ending of string
	
	while (i < j) {				// while indices did not cross each other yet
		
		swap(&str[i], &str[j]);	// swap the characters where the indices are at, and move each index one step to the middle
		i = i+1;
		j = j-1;
		
		
	}
	return str;
	
}


// used to convert an int value to a string (while keeping its value) assuming its base 10
char* itoa(int number, char *str ) {
	
	int isNegative;					// flag used to determine if given number is negative or not
	int temp;						// this will store the absolute value of given number
	int i = 0;						// index for given string
	
	if (number < 0) {
		isNegative = 1;
		temp = number * -1;

	}
		
	else {
		isNegative = 0;
		temp = number;
	}
	
	
	while (temp) {
		
		int j = temp % 10;
		str[i] = 48+j;			// 48 is the ASCII for '0'
		i = i+1;
		temp = temp/10;

		
	}
	
	if (i == 0) {				// if given number is 0, 'i' would not move and remain at 0 (while loop doesn't execute)
		
		str[i] = '0';
		i = i+1;
		
	}
	
	if (isNegative) {			// add the minus sign to the string if it was negative
		
		str[i] = '-';
		i = i+1;
	}
	
	str[i] = '\0';				// end the string
	
	return flip(str,strlen(str));
	
}

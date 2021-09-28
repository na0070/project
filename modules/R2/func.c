
// #include "structures.h"
// #include "../mpx_supt.h"
// #include <string.h>


// constants to be used throughout functions
// #define READY 0
// #define BLOCKED 1

// #define NOT_SUSPENDED 0
// #define SUSPENDED 1

// #define SYSTEM 0
// #define APPLICATION 1


// where all the functions of R2 would be (along with itoa())
//also where all the queues are initialized for function use

#include "func.h"
struct queue ready = {.head = NULL, .tail = NULL};					// initializing the ready queue


struct queue blocked = {.head = NULL, .tail = NULL};				// initializing the blocked queue


struct queue sus_ready = {.head = NULL, .tail = NULL};				// initializing the suspended ready queue

struct queue sus_blocked = {.head = NULL, .tail = NULL};			// initializing the suspended blocked queue



/*
FindPCB() is given a name of a PCB. Searches all queues until it finds a match.
When it does, returns a pointer to the PCB of the same name. If no match is found, return NULL
*/
pcb* findPCB(char* name) {
	
	pcb* ptr = ready.head;					// the indexing pointer (used for traversing each queue, starting with ready queue)
	
	while (ptr != NULL) {					// loop through ready queue
		print("looking in ready\n",20);
		if (strcmp(name, ptr->name) == 0){	// if given name matches current PCB's name
			print("found in ready\n", 20);
			return ptr;						// return the pointer (i.e PCB found)
		
		}
		ptr = ptr->next;					// go to next PCB in queue
	}
	
	print("not in ready\n",14);
	// not found in ready, try in blocked
	
	ptr = blocked.head;						
	
	while (ptr != NULL) {					// loop through blocked queue
		print("looking in block\n",20);
		if (strcmp(name, ptr->name) == 0) 	// if given name matches current PCB's name
			return ptr;						// return the pointer (i.e PCB found)
		
		
		ptr = ptr->next;					// go to next PCB in queue
	}
	print("not in blocked\n",16);
	
	// not found in blocked, try in suspended ready
	
	ptr = sus_ready.head;				// the indexing pointer
	
	while (ptr != NULL) {					// loop through sus_ready queue
		print("looking in Sready\n",20);
		if (strcmp(name, ptr->name) == 0) 	// if given name matches current PCB's name
			return ptr;						// return the pointer (i.e PCB found)
		
		
		ptr = ptr->next;					// go to next PCB in queue
	}
	
	print("not in s_ready\n",16);
	// not found in suspended ready, try in suspended blocked
	
	ptr = sus_blocked.head;				// the indexing pointer
	
	while (ptr != NULL) {					// loop through sus_ready queue
		print("looking in Sblock\n",20);
		if (strcmp(name, ptr->name) == 0) 	// if given name matches current PCB's name
			return ptr;						// return the pointer (i.e PCB found)
		
		
		ptr = ptr->next;					// go to next PCB in queue
	}
	
	print("not in s_blocked\n",20);
	// not found in any queue, then PCB doesn't exist, return null
	return NULL;
}


pcb* allocatePCB() {
	pcb* alloc = (pcb*)sys_alloc_mem(sizeof(pcb)); // initialized a pcb pointer, allowing pointer to point at a specific location of memory (memory is the size of pcb)
	alloc->prio=5;
	alloc->susp=0;
	// alloc->stackTop = alloc->stack[1023];
	// alloc->stackBase = alloc->stack[0];
	return alloc;
}



pcb* setupPCB(char* newName, unsigned char newClass, int newPriority) {	
print("inside setUp\n",15);	
	// setupPCB takes 3 parameters: name, class and priority of the new PCB to be created. Either returns the new pcb pointer if successful, or NULL if there is an error
	// perform error checking first (are given values valid?)
	
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
	if (newPCB == NULL) print("newPCB is null\n",16);
	else print("new pcb not null\n",19);
	return newPCB;													// after all the initialization is complete, return the new pcb pointer to wherever this function was used
}



char* freePCB( pcb* name ){

return (char*)sys_free_mem(name );// return free memmory function
}// free pcb end 



void insertPCB( pcb* pntr) {
s
	print("inside insert\n",16);
	if (findPCB(pntr->name)  == NULL) {				// checking if PCB exist 
		print("after if\n",10);
		int state = pntr->state; 										// taking the pointer state 
			int priorty = pntr->prio; 									// taking the pointer priorty 
				int sus = pntr->susp;									// taking the pointer suspend status 
				
				struct queue* Q;
				
				if ( state == READY){
					
					//if (sus == SUSPENDED){
						
					if (sus == SUSPENDED)
						Q = &sus_ready;								// also tried without '&' 
					else
						Q = &ready;
					
					
						pcb* Qpntr = Q->head;
						
						if (Q->head == NULL) {	// if queue is empty
							// Q->head = pntr;
							// Q->tail = pntr;
							ready.head = pntr;
							ready.tail = pntr;
							pntr->next = NULL;
							pntr->prev = NULL;
							// sus_ready.head == pntr;
							// sus_ready.tail == pntr;
							print("inserted in empty ready.",24);
							
						}
							else {
						
								//pcb* Qpntr = sus_ready.head; 
								
								while (Qpntr != NULL) {
									
									if (priorty <= Qpntr->prio){
										
										Qpntr = Qpntr->next;
									}
									else  {
										pntr->next = Qpntr;
										pntr->prev = Qpntr->prev;
										
										Qpntr->prev->next = pntr;
										Qpntr->prev = pntr;
										print("inserted in non_empty ready.",29);
										break;
									}
									
								}
							}
					//}
					// else {
						
						// Q = ready;
						
						// pcb* Qpntr = ready.head; 
						
						// while (Qpntr != NULL) {
							
							// if (priorty <= Qpntr->prio){
								
								// Qpntr = Qpntr->next;
							// }
							// else  {
								// pntr->next=Qpntr; 
							// }
							
						// }
						
					// }
				} else {									// in blocked (FIFO)
				
				
						if (state == SUSPENDED)
							Q = &sus_blocked;
						else
							Q = &blocked;
						
						//pcb* Qpntr = NULL;
						
						
						if (Q->head == NULL) {
							
							Q->head = pntr;
							Q->tail = pntr;
							pntr->next = NULL;
							pntr->prev = NULL;
							print("inserted in empty blocked.",27);
							
						}
						else {
							
							pntr->prev = Q->tail;
							pntr->prev->next = pntr;
							Q->tail = pntr;
							pntr->next = NULL;
							print("inserted in non_empty blocked.",32);
							
						}
						
						
							
								// if (sus == SUSPENDED){
										
										// sus_blocked.tail = pntr;
											// pntr->next = NULL;
									// } else {
										
										// blocked.tail = pntr;
											// pntr->next = NULL;
									// }
				}
				
				Q->count = Q->count+1;
				print("end of insert\n",16);
	} 
	else
		print("ERROR: PCB already exists.",27);
	
}

int removePCB (pcb* pntr) {
	
	if (pntr == NULL) {
		
		return -1; 												// Error code
		
	} else 
			{
	
		pntr->prev->next = pntr->next;            // instead of A pointing to B, A pointing to C
			pntr->next->prev = pntr->prev; 		// instead of C pointing to B, C pointing to A
			
			pntr->next = NULL; 							// unlink next pointer of B "C"
		pntr->prev = NULL;								// unlink previous pointer of B "A"
		
	}  return 0; 												// success
			
}



// showPCB prints information for a single pcb (using the pointer)
void showPCB(pcb* ptr) {
	
	//sys_req(WRITE,DEFAULT_DEVICE,"\nName: ",sizeof("\nName: "));
	print("\nName: ",8);
	//sys_req(WRITE,DEFAULT_DEVICE,ptr->name,20);									// print the name
	print(ptr->name,20);
	
	//sys_req(WRITE,DEFAULT_DEVICE,"\nClass: ",9);
	print("\nClass: ",9);
	
	if (ptr->class == SYSTEM)													// print the class (based on value)
		//sys_req(WRITE,DEFAULT_DEVICE,"SYSTEM",7);
		print("SYSTEM",7);
	
	else
		//sys_req(WRITE,DEFAULT_DEVICE,"APPLICATION",12);
		print("APPLICATION",12);
	
	
	//sys_req(WRITE,DEFAULT_DEVICE,"\nState: ",9);
	print("\nState: ",9);
	
	if (ptr->state == READY)													// print the state (based on value)
		//sys_req(WRITE,DEFAULT_DEVICE,"READY",7);
	print("READY",7);
	
	else
		//sys_req(WRITE,DEFAULT_DEVICE,"BLOCKED",8);
		print("BLOCKED",8);
	
	
	//sys_req(WRITE,DEFAULT_DEVICE,"\nStatus: ",10);
	print("\nStatus: ",10);
	
	if (ptr->susp == NOT_SUSPENDED)													// print the status (based on value)
		//sys_req(WRITE,DEFAULT_DEVICE,"NOT SUSPENDED",14);
		print("NOT SUSPENDED",14);
		
	else
		//sys_req(WRITE,DEFAULT_DEVICE,"SUSPENDED",10);
		print("SUSPENDED",10);
	
	
	//sys_req(WRITE,DEFAULT_DEVICE,"\nPriority: ",12);
	print("\nPriority: ",12);
	
	char prioNum[1];
	itoa(ptr->prio,prioNum);
	
	//sys_req(WRITE,DEFAULT_DEVICE,prioNum,strlen(prioNum));							// print the priority (based on value)
	print(prioNum,strlen(prioNum));
	
}
void showqueue(struct queue allpcbs){						 //function used to print an entire queue using the function showpcb
	pcb* ptr=allpcbs.head; 									//set a pcb pointer to the head of a given queue
	while(ptr!=NULL){										 //while still not reaching end of the queue
		showPCB(ptr); 										//print the current pcb's info
		ptr=ptr->next;									 //go to next pcb in queue
		
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
	
	if (i == 0) {				// if given number is 0, i would not move and remain at 0 (while loop doesn't execute)
		
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
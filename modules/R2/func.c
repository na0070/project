
#include "structures.h"
#include "../mpx_supt.h"
#include <string.h>


// where all the functions of R2 would be (along with itoa())



/*
FindPCB() is given a name of a PCB. Searches all queues until it finds a match.
When it does, returns a pointer to the PCB with the same name
*/
// pcb* FindPCB(char* name) {
	// make a temp pointer
	// loop through all queues (each queue in a separate loop)
	// compare each PCB's name with the given name (using strcmp(name, "PCB"->name) == 0 )
	// if a match is found, get the temp pointer to it
	// return the pointer
	// if no match, return NULL
	
	// /*
	// if (match)
		// return temp pointer;
	
	// else
		// return NULL;
	
	
	// */
	
	
	
// }


pcb* allocatePCB() {
	pcb* alloc = (pcb*)sys_alloc_mem(sizeof(pcb)); // initialized a pcb pointer, allowing pointer to point at a specific location of memory (memory is the zie of pcb)
	alloc->prio=5;
	alloc->susp=0;
	return alloc;
}






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
char* FreePCB( pcb* name ){

return sys_free_mem(name );// return free memmory function
}// free pcb end 

void InsertPCB( pcb* pntr) {

	if (FindPCB(pntr->name)  == NULL) {				// checking if PCB exist 
	
		int state = pntr->state; 										// taking the pointer state 
			int priorty = pntr->prio; 									// taking the pointer priorty 
				int sus = pntr->susp;									// taking the pointer suspend status 
				
				if ( state == READY){
					
					if (sus == SUSPENDED){
						
						struct queue* Qpntr = sus_ready.head; 
						
						while (Qpntr != NULL) {
							
							if (priorty <= Qpntr->prio){
								
								Qpntr = Qpntr->next;
							}
							else  {
								pntr->next=Qpntr; 
							}
							
						}
					}
					else {
						
						struct queue* Qpntr = ready.head; 
						
						while (Qpntr != NULL) {
							
							if (priorty <= Qpntr->prio){
								
								Qpntr = Qpntr->next;
							}
							else  {
								pntr->next=Qpntr; 
							}
							
						}
						
					}
				} else {
								if (sus == SUSPENDED){
										
										sus_blocked.tail = pntr;
											pntr->next = NULL;
									} else {
										
										blocked.tail = pntr;
											pntr->next = NULL;
									}
				}
	} 
	
}



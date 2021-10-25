#include "func5.h"


//global variable: start location of heap
unsigned char* start_memory;    

// global list of free/allocated lists

struct list allocatedList = {.head = NULL, .tail = NULL};
struct list freeList = {.head = NULL, .tail = NULL};

struct list memoryList = {.head = NULL, .tail = NULL};	// if using one joint list approach

// initiallie the heap (that will then be divided into more blocks)
void init_heap(int size) {
  
  start_memory = (void *)kmalloc(size + sizeof(CMCB) + sizeof(LMCB));
  // put CMCB at top, LMCB at bottom
  CMCB* heap_head = (CMCB*)start_memory;
  LMCB* heap_end = (LMCB*)( start_memory + size + sizeof(CMCB) ); // size of LMCB already baked in to the memory block, so no need to subtract its size


heap_head -> size = size;	// "size" of the block the CMCB is within, "size" indicates the block's total size, dont include CMCB and LMCB here

  // initialize the heap as free at first
  heap_head -> type = FREE;
  heap_end -> type = FREE;
  
  // set beginning address of CMCB
  heap_head -> address = start_memory + sizeof(CMCB);

  //initialize the block lists
 freeList.head = heap_head;
 freeList.tail = heap_head;		// no need for a tail

// heap_head is the only one in the list so far, so no other CMCBs exist yet
 heap_head -> next = NULL;
 heap_head -> prev = NULL;

 // allocated head and tail are already NULL

}
  
void allocateMemory(int size) {
  //allocate memory with size + sizeof(CMCB) + sizeof(LMCB)
  // use first-fit method to look for correct size block
  // divide block if needed to allocated and free (remainder)
  
  int needed_size = size + sizeof(CMCB) + sizeof(LMCB);

  CMCB* ptr = freeList -> head; // index pointer
  //CMCB* ptr = memoryList.head;	// if using one list approach

	while (ptr != NULL) {

		if (ptr -> size >= needed_size) {
			// found correct size, remove it from list, change it to allocated and make the remainder free
			
			ptr -> type = ALLOCATED;		// set the block to be allocated
			
			// split the block to allocated and free (remainder)
			LMCB* newLMCB = (LMCB*)(ptr + size + sizeof(CMCB));	// set new LMCB to end of new block
			newLMCB -> type = FREE;

			CMCB* newFree = (CMCB*) (newLMCB + sizeof(LMCB)); // set new CMCB for remainder block

			newFree -> type = FREE;
			newFree -> size = ptr -> size - size - sizeof(CMCB) - sizeof(LMCB);	// set new size of remainder block
			newFree -> address = newFree + sizeof(CMCB);		// set new address of remainder
			
			// name is not important (for now)
			ptr -> size = size;		// readjust the allocated block's size
			// would we need to adjust any old LMCBs?

			if (allocatedList.head == NULL) {		// may want to stick with using one list, so may want to remove this if-statement
				// set the head and tail of the empty list to be the new block to be added
				allocatedList.head = ptr;
				allocatedList.tail = ptr;

			}

			else {
				// reconnect blocks to list correctly (using one list approach)
				newFree -> prev = ptr;
				newFree -> next = ptr->next;

				ptr-> next = newFree;

			}

			break;

		}
    	ptr = ptr -> next;
	}
  

}

void freeMemory (CMCB* pntr) {
	
	if (pntr -> type == FREE) {
		
		print ("The memory already free \n", 26 );
	}
	else {
		
		pntr -> prev -> next = pntr -> next; 
		
			pntr -> next -> perv = pntr -> perv ;
			
			pntr -> next = NULL;
			
				pntr -> perv = NULL; 
				
				pntr -> type = FREE; 
				
				CMCB* temp = freeList.head;
			
			while (temp != NULL) {
				
				if (pntr -> address > temp -> address){
					
					pntr -> next = temp -> next; 
					
						pntr -> perv = temp;
						
					temp -> next = pntr; 
					
				pntr -> next -> perv = pntr; 
				
				break; 
				}
				
				temp = temp -> next; 
			}
			
			if (pntr -> type == pntr -> prev-> type) {
				
				CMCB* newBlock;
				
				newBlock -> size = pntr -> size + pntr -> prev -> size + sizeof(CMCB) + sizeof(LMCB);
				
				newBlock -> next = pntr -> next; 
				
				newBlock -> prev = pntr -> prev -> prev;
				
				newBlock -> next -> prev = newBlock; 
				
				newBlock -> prev -> next = newBlock; 
				
				pntr -> prev -> prev = NULL;
				
				pntr -> prev -> next = NULL; 
				
				pntr -> prev = NULL; 
				
				pntr -> next = NULL; 
				
				
			}
			
	}
}
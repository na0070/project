#include "func5.h"


//global variable: start location of heap (better to use u32int* ?)
u32int* heap;    
//u32int* heap;

// global list of free/allocated lists

// struct list allocatedList = {.head = NULL, .tail = NULL};
// struct list freeList = {.head = NULL, .tail = NULL};

struct list memoryList = {.head = NULL, .tail = NULL};	// if using one joint list approach

// initiallie the heap (that will then be divided into more blocks)
void init_heap(int size) {
  
	heap = (void *)kmalloc(size + sizeof(CMCB) + sizeof(LMCB));
	// put CMCB at top, LMCB at bottom
	CMCB* heap_head = (CMCB*) heap;
	//LMCB* heap_end = (LMCB*)( heap + size + sizeof(CMCB) ); // size of LMCB already baked in to the memory block, so no need to subtract its size


	// "size" of the block the CMCB is within, "size" indicates the block's total size, dont include CMCB and LMCB here
	heap_head -> size = size;	

	// initialize the heap as free at first
	heap_head -> type = FREE;
	//heap_end -> type = FREE;
	
	// set beginning address of CMCB (starting from first addressable location, AKA the heap)
	heap_head -> address = heap + sizeof(CMCB);

	//initialize the block lists
	memoryList.head = heap_head;
	memoryList.tail = heap_head;		// no need for a tail (maybe)

	// heap_head is the only one in the list so far, so no other CMCBs exist yet
	heap_head -> next = NULL;
	heap_head -> prev = NULL;


}
// return 0 if memory was allocated, and return -1 if not allocated (not enough space found)
int allocateMemory(int size) {
  // allocate memory with size + sizeof(CMCB) + sizeof(LMCB)
  // use first-fit method to look for correct size block
  // divide block if needed to allocated and free (remainder)
  
  int needed_size = size + sizeof(CMCB) + sizeof(LMCB);

//   CMCB* ptr = freeList -> head; // index pointer (if using 2 lists approach)
  CMCB* ptr = memoryList.head;	// if using one list approach

	while (ptr != NULL) {

		// check if current block is of sufficient size
		if (ptr -> size >= needed_size) {
			
			ptr -> type = ALLOCATED;		// set the block to be allocated
			
			// split the block to allocated and free (remainder)
			LMCB* newLMCB = (LMCB*)(ptr + size + sizeof(CMCB));	// set new LMCB to end of new block
			newLMCB -> type = FREE;



			CMCB* newFree = (CMCB*) (newLMCB + sizeof(LMCB)); // set new CMCB for remainder block

			newFree -> type = FREE;
			newFree -> size = ptr -> size - size - sizeof(CMCB) - sizeof(LMCB);	// set new size of remainder block
			newFree -> address = (u32int)newFree + sizeof(CMCB);		// set new address of remainder
			
			// name is not important (for now)
			ptr -> size = size;		// readjust the allocated block's size
			// would we need to adjust any old LMCBs?

			// if (allocatedList.head == NULL) {		// may want to stick with using one list, so may want to remove this if-statement
			// 	// set the head and tail of the empty list to be the new block to be added
			// 	allocatedList.head = ptr;
			// 	allocatedList.tail = ptr;

			// }

			// else {
				// reconnect blocks to list correctly (using one list approach)
				newFree -> prev = ptr;
				newFree -> next = ptr->next;

				ptr-> next = newFree;

			// }

			return 0;		// stop searching

		}
    	ptr = ptr -> next;		// if not enough spcae, go to next block and check
	}
  
  return -1;

}

void freeMemory (u32int address) {

	CMCB* pntr = memoryList.head;

	while (pntr != NULL) {

		
		if (pntr -> address = address) {

			if (pntr -> type == FREE) {
		
			print ("The memory already free \n", 26 );
		
			}

			else {
				/*
				pntr -> prev -> next = pntr -> next; 
			
				pntr -> next -> perv = pntr -> perv ;
				
				pntr -> next = NULL;
				
					pntr -> perv = NULL; 
					
					pntr -> type = FREE; 
					
					CMCB* temp = freeList.head; */
					
					pntr -> type = FREE; 
				
				/* while (temp != NULL) {
					
					if (pntr -> address > temp -> address){
						
						pntr -> next = temp -> next; 
						
							pntr -> perv = temp;
							
						temp -> next = pntr; 
						
					pntr -> next -> perv = pntr; 
					
					break; 
					} 
					
					temp = temp -> next; 
				}*/
				
				//CMCB* newBlock;
				
				if (pntr -> type == pntr -> next-> type){
					
					pntr -> size = pntr -> size  + pntr -> next -> size + sizeof(CMCB) + sizeof(LMCB);
					
					//pntr -> next -> next = NULL;
					pntr -> next -> prev = NULL;

					pntr -> next = pntr -> next -> next;

					pntr -> next -> prev = pntr;
					
					//newBlock -> prev = pntr -> prev;
					
					// newBlock -> next -> prev = newBlock; 
					
					// newBlock -> prev -> next = newBlock; 
					
					// pntr -> next -> prev = NULL; 
					
					// pntr -> next -> next = NULL; 
					
					// pntr -> prev = NULL;
					
					// pntr -> next = NULL; 
					
				}


				if (pntr -> type == pntr -> prev-> type) {
					
					pntr -> prev -> size = pntr -> size + pntr -> prev -> size + sizeof(CMCB) + sizeof(LMCB);
					
					pntr -> prev -> next = pntr -> next; 

					pntr -> next -> prev = pntr -> prev;
					
					// newBlock -> prev = pntr -> prev -> prev;
					
					// newBlock -> next -> prev = newBlock; 
					
					// newBlock -> prev -> next = newBlock; 
					
					// pntr -> prev -> prev = NULL;
					
					// pntr -> prev -> next = NULL; 
					
					pntr -> prev = NULL; 
					
					pntr -> next = NULL; 
					
				}
			
			
			}
			break;
		}

		pntr = pntr -> next;
	}
	
	
}
#include "func5.h"


//global variable: start location of heap
(unsigned char)* start_memory;    

// global list of free/allocated lists

struct list allocatedList = {.head = NULL, .tail = NULL};
struct list freeList = {.head = NULL, .tail = NULL};

// initiallie the heap (that will then be divided into more blocks)
void init_heap(int size) {
  
  start_memory = (void *)kmalloc(size + sizeof(CMCB) + sizeof(LMCB));
  // put CMCB at top, LMCB at bottom
  CMCB* heap_head = (CMCB*)start_memory;
  LMCB* heap_end = (LMCB*)( start_memory + size + sizeof(CMCB) ); // size of LMCB already baked in to the memory block, so no need to subtract its size

  // initialize the heap as free at first
  heap_head -> type = FREE;
  heap_end -> end = FREE;
  

  //initialize the block lists
 freeList.head = heap_head;
 freeList.tail = heap_head;

// heap_head is the only one in the list so far, so no other CMCBs exist yet
 heap_head -> next = NULL;
 heap_head -> prev = NULL;


 // returns anything?
 return;
}
  
void allocateMemory(int size) {
  //allocate memory with size + sizeof(CMCB) + sizeof(LMCB)
  // use first-fit method to look for correct size block
  // divide block if needed to allocated and free (remainder)
  
  int needed_size = size + sizeof(CMCB) + sizeof(LMCB);

  CMCB* ptr = freeList -> head; // index pointer

  while (ptr != NULL) {

    if (ptr -> size >= needed_size) {
      // found correct size, remove it from list, change it to allocated and make the remainder free
      ptr -> type = ALLOCATED;

      if (allocatedList.head == NULL)
        allocatedList.head = ptr;

      // reconnect blocks to list correctly

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
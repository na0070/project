#include "func5.h"
#include "../R2/func.h"
#include "../mpx_supt.h"


//global variable: start location of heap
u32int* heap_head;    

// global list of memory

struct list memoryList = {.head = NULL, .tail = NULL};	// if using one joint list approach

// initiallie the heap (that will then be divided into more blocks)
u32int* init_heap(int size) {
  
	heap_head = (void *)kmalloc(size + sizeof(CMCB) + sizeof(LMCB));
	// put CMCB at top, LMCB at bottom
	CMCB* heap_CMCB = (CMCB*) heap_head; // assign a CMCB to the heap's head


	// "size" of the block the CMCB is within, "size" indicates the block's total size, dont include CMCB and LMCB here
	heap_CMCB -> size = size;	

	// initialize the heap as free at first
	heap_CMCB -> type = FREE;
	
	// set beginning address of CMCB (starting from first addressable location, AKA the heap)
	heap_CMCB -> address = (u32int)heap_CMCB + sizeof(CMCB);

	//initialize the block lists
	memoryList.head = heap_CMCB;
	memoryList.tail = heap_CMCB;		// no need for a tail (maybe)

	// heap_head is the only one in the list so far, so no other CMCBs exist yet
	heap_CMCB -> next = NULL;
	heap_CMCB -> prev = NULL;

  // print("heap initialized\n",17);
  return heap_head;

}
// return address of allocated memory for requesting process
u32int allocateMemory(int size) {
  // allocate memory with size + sizeof(CMCB) + sizeof(LMCB)
  // use first-fit method to look for correct size block
  // divide block if needed to allocated and free (remainder)
  int needed_size = size + sizeof(CMCB) + sizeof(LMCB);

  CMCB* ptr = memoryList.head;	// start pointing from beginning of memory

	while (ptr != NULL) {

    if (ptr -> type == FREE) {               // check if current block is free before checking for the size

		//edge case: if a free block can fit a process but not needing to split the block 
		if (ptr -> size >= size && ptr -> size <= needed_size) {
			ptr -> type = ALLOCATED;               // only change the block's type to ALLOCATED and return its address
			return (u32int)ptr -> address;
		}

  		if (ptr -> size >= needed_size) {     // check if current block is of sufficient size (with remainder)
  			
  			ptr -> type = ALLOCATED;		// set the block to be allocated
  			
  			// split the block to allocated and set the remainder to free
  			LMCB* newLMCB = (LMCB*)((u32int)ptr + size + sizeof(CMCB));	// set new LMCB to end of allocated block
  			newLMCB -> type = FREE;



  			CMCB* newFree = (CMCB*) ((u32int)newLMCB + sizeof(LMCB)); // set new CMCB for start of remainder block

  			newFree -> type = FREE;
  			newFree -> size = ptr -> size - size - sizeof(CMCB) - sizeof(LMCB);	// set new size of remainder block
        	newFree -> address = (u32int)newFree + sizeof(CMCB);    // set new address of remainder
        
  			
  			ptr -> size = size;		// readjust the allocated block's size

  				// reconnect blocks to list correctly (using one list approach)
  				newFree -> prev = ptr;
  				newFree -> next = ptr->next;
          newFree -> next -> prev = newFree;

  				ptr-> next = newFree;


  			return (u32int)ptr -> address;		// stop searching and return the address of memory

  		}
	}
    	ptr = ptr -> next;		// if not enough spcae, go to next block and check
	}
  return 0;

}

void freeMemory (u32int address) {
	CMCB* pntr = memoryList.head;

	while (pntr != NULL) {
		
		if (pntr -> address == address) {

			if (pntr -> type == FREE) {
		
			print ("The memory already free \n", 26 );
		
			}

			else {
					
					pntr -> type = FREE; 

				
				if (pntr -> type == pntr -> next-> type){
					
					pntr -> size = pntr -> size  + pntr -> next -> size + sizeof(CMCB) + sizeof(LMCB);

					pntr -> next -> prev = NULL;

					pntr -> next = pntr -> next -> next;

					pntr -> next -> prev = pntr;
					
				}


				if (pntr -> type == pntr -> prev-> type) {
					
					pntr -> prev -> size = pntr -> size + pntr -> prev -> size + sizeof(CMCB) + sizeof(LMCB);
					
					pntr -> prev -> next = pntr -> next; 

					pntr -> next -> prev = pntr -> prev;
					
					pntr -> prev = NULL; 
					
					pntr -> next = NULL; 
					
				}
			
			
			}
			break;
		}

		pntr = pntr -> next;
	}
	
	
}

// check if nothing is allocated from heap
int isEmpty() {

  CMCB* ptr = memoryList.head;  // index pointer starting at head

  while (ptr != NULL) {   // looping through list 

    if (ptr->type != FREE)  // if this block is not free
      return FALSE;         // then heap is not empty, return false and exit

    ptr = ptr->next;        // if it is free, check next block

  } // end of list

  return TRUE;            // if could not find allocated blocks, then heap is empty


}

// print block information (size and address) of the chosen type in the heap
void showList(int printType) {
  CMCB* ptr = memoryList.head;    // index pointer starting at head
  char blockAddress[20];        // string holding block's address value
  char blockSize[20];           // string holding block's size value

  print("\x1B[93m",1);          // print in yellow

  while (ptr != NULL) {         // looping through list
    if (ptr->type == printType) { // if this block's type is the one to print

      itoa((int)ptr->address,blockAddress);    // convert address value into a string
      itoa(ptr->size,blockSize);          	   // convert size value into a string

      print("Block address: ",16);           // print the address
      print(blockAddress,sizeof(blockAddress));
      print("\n",1);

      print("Block size: ",13);              // print the size
      print(blockSize,sizeof(blockSize));
      print("\n",1);

      print("\x1B[92m************************\n\x1B[93m",28);     // to visually distinguish between different block prints

    }

    ptr = ptr->next;            // go to next block in the list

  }

  print("\x1B[39m",1);          // default color


}
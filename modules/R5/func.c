#include "func.h"


//global variable: start location of heap
(unsigned char)* start_memory;

void init_heap(int size) {
  
  start_memory = (void *)kmalloc(size + sizeof(CMCB) + sizeof(LMCB));
  // put CMCB at top, LMCB at bottom
  CMCB* heap_head = (CMCB*)start_memory;
  LMCB* heap_end = start_memory + size + sizeof(CMCB);
  
}
  
void allocateMemory(int size) {
  //allocate memory with size + sizeof(CMCB) + sizeof(LMCB)
  
  
  
}

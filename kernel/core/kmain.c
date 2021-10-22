/*
  ----- kmain.c -----

  Description..: Kernel main. The first function called after
      the bootloader. Initialization of hardware, system
      structures, devices, and initial processes happens here.
      
      Initial Kernel -- by Forrest Desjardin, 2013, 
      Modifications by:    Andrew Duncan 2014,  John Jacko 2017
      				Ben Smith 2018, and Alex Wilson 2019
*/
#include "modules/R1/comm.h"
#include <stdint.h>
#include <string.h>
#include <system.h>

#include <core/io.h>
#include <core/serial.h>
#include <core/tables.h>
#include <core/interrupts.h>
#include <mem/heap.h>
#include <mem/paging.h>

#include "modules/mpx_supt.h"
#include "modules/R2/func.h"
#include "modules/R4/Alarm.h"

void infinite();

void kmain(void)
{
   extern uint32_t magic;
   // Uncomment if you want to access the multiboot header
   // extern void *mbd;
   // char *boot_loader_name = (char*)((long*)mbd)[16];

  
   // 0) Initialize Serial I/O 
   // functions to initialize serial I/O can be found in serial.c
   // there are 3 functions to call
   
   init_serial(COM1);
   set_serial_out(COM1);
   set_serial_in(COM1);
 
   klogv("Starting MPX boot sequence...");
   klogv("Initialized serial I/O on COM1 device...");

   // 1) Initialize the support software by identifying the current
   //     MPX Module.  This will change with each module.
   // you will need to call mpx_init from the mpx_supt.c
   
   mpx_init(MODULE_R4);
 	
   // 2) Check that the boot was successful and correct when using grub
   // Comment this when booting the kernel directly using QEMU, etc.
   if ( magic != 0x2BADB002 ){
     //kpanic("Boot was not error free. Halting.");
   }
   
   // 3) Descriptor Tables -- tables.c
   //  you will need to initialize the global
   // this keeps track of allocated segments and pages
   klogv("Initializing descriptor tables...");
   
   init_gdt();
   init_idt();
   

    // 4)  Interrupt vector table --  tables.c
    // this creates and initializes a default interrupt vector table
    // this function is in tables.c
    
    init_pic();
    init_irq();
    sti();
    
    klogv("Interrupt vector table initialized!");
    
   // 5) Virtual Memory -- paging.c  -- init_paging
   //  this function creates the kernel's heap
   //  from which memory will be allocated when the program calls
   // sys_alloc_mem UNTIL the memory management module  is completed
   // this allocates memory using discrete "pages" of physical memory
   // NOTE:  You will only have about 70000 bytes of dynamic memory
   //
   klogv("Initializing virtual memory...");
   
   init_paging();

	

   // 6) Call YOUR command handler -  interface method
   klogv("Transferring control to commhand...");



/*
  testing R3 and R4
  commhand() is commented for purposes of R4
*/

	// commhand();

// the 3 lines below should be commented if only testing R3. Uncomment the above line instead
   loadr_pcb("command_handler", SYSTEM, NOT_SUSPENDED, 9, (u32int)commhand);     // loads commhand as a process
   loadr_pcb("idle_process", SYSTEM, NOT_SUSPENDED, 1, (u32int)idle);            // loads idle as a process
   loadr_pcb("alarm_process", SYSTEM, NOT_SUSPENDED, 1, (u32int)checkTime);      // loads alarm checking process
   loadr_pcb("infinite_process", SYSTEM, NOT_SUSPENDED, 1, (u32int)infinite);   // loads infinite command
   
   asm volatile("int $60");







   // 7) System Shutdown on return from your command handler
   klogv("Starting system shutdown procedure...");
   
   /* Shutdown Procedure */
   klogv("Shutdown complete. You may now turn off the machine. (QEMU: C-a x)");
   hlt();
}
void infinite(){
while(1){
sys_req(IDLE, DEFAULT_DEVICE, NULL, NULL);
print("infinte command massage\n",25);

}// end of the loop 


}// end of infinte 


#include "../mpx_supt.h"

#include <stdint.h>
#include <string.h>


#include <core/io.h>
#include <core/serial.h>
#include "comm.h" //its header file

//main function

void commhand() {
	
	char buffer[80];	// initialize the buffer
	int size = 80;
	
	while (strcmp(buffer,"end") != 0) {		// Keep asking while user did not enter "end"
		memset(buffer,'\0',size);			// set aside memory for the buffer (and wipe it clean)
		sys_req(READ,DEFAULT_DEVICE,buffer,&size);	// goes to polling
		
		serial_print("result: ");
		sys_req(WRITE,DEFAULT_DEVICE,buffer,&size);	//returned from polling
		serial_print("\n");
		
		
	
	} // leaving commhand
	
	serial_print("Ending commhand()...\n");
	
	return;

}
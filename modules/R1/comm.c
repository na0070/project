
#include "../mpx_supt.h"
#include "comm.h"

#include <stdint.h>
#include <string.h>

#include <core/io.h>
#include <core/serial.h>


void commhand() {
	
	
	int size = 80;
	char buffer[size];	// initialize the buffer
	
	
	
	
	while (TRUE) {							// Keep asking for inputs
		memset(buffer,'\0',size);			// set aside memory for the buffer (and wipe it clean)
		
		serial_print("Welcome to MPX_core main menu...\n");
		serial_print("You can choose from one of the following options by typing its keyword:\n");
		serial_print("-version\n");
		serial_print("-help\n");
		serial_print("-getdate\n");
		serial_print("-setdate\n");
		serial_print("-gettime\n");
		serial_print("-settime\n");
		serial_print("-shutdown\n");
		// possibly adding more commands if needed
		
		sys_req(READ,DEFAULT_DEVICE,buffer,&size);	// goes to polling
		
		
		char *token;				// the word(s) separated from the buffer
		const char split[] = " ";	// the character to indicate splitting of the tokens
		
		token = strtok(buffer,split);	// use strtok() to split the first word from the buffer
		
		// if (strcmp(token,"date") == 0) {		// for testing purposes
			
			
			// while (token != NULL) {
				// serial_print(token);
				// serial_print("\n");
				// token = strtok(NULL, split);
			// }
			
		// }
		
		if (strcmp(token,"s") == 0) { // testing
			
			
			// outb(0x70,0x00);
			// unsigned char i = inb(0x71);
			
			// buffer[0] = i;
			// serial_print((const char *)&i);
		}
		
		
		else if (strcmp(token,"version") == 0) {		// version command
			
			serial_print("version() in progress.. check back later\n");
		}
		
		
		else if (strcmp(token,"help") == 0) {		// help command
			
			serial_print("help() in progress.. check back later\n");
		}
		
		else if (strcmp(token,"date") == 0) {		// date(s) command(s)
			
			serial_print("date(s) in progress.. check back later\n");
		}
		
		else if (strcmp(token,"time") == 0) {		// time(s) command
			
			serial_print("time(s) in progress.. check back later\n");
		}
		
		else if (strcmp(token,"shutdown") == 0) {		// shutdown command
			
			serial_print("shutdown() in progress.. check back later\n");
		}
		
		//sys_req(WRITE,DEFAULT_DEVICE,buffer,&size);	//returned from polling
		
		else if (strcmp(token,"end") == 0) {	// if user entered "end"
			
			serial_print("Ending commhand()...\n");	// leave commhand() and begin shutdown
			break;
			
		}
		
		else if (strcmp(token,"clear") == 0) {		// clearing the screen
			
			serial_print("\x1B[2J"); // clear screen
			serial_print("\x1B[H"); // send cursor back to (0,0) position (top left corner)
			
		}
		
		else {
			
			serial_print("unknown command entered. Please check spelling and/or syntax..\n");
			
		}
		
		serial_print("\n");
	
	} // leaving commhand
	
	
	
	return;

}
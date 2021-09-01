
#include "../mpx_supt.h"
#include "comm.h"
#include "getsetdate.h"
#include "getandsettime.h"

#include <stdint.h>
#include <string.h>

#include <core/io.h>
#include <core/serial.h>


void commhand() {
	
	
	int size = 80, sizeBuff = 7;
	char buffer[size];	// initialize the buffer
	int dateBuff[sizeBuff];	// initialize date buffer
	int timeBuff[sizeBuff];	// initialize time buffer
	
	
	
	while (TRUE) {							// Keep asking for inputs
		memset(buffer,'\0',size);			// set aside memory for the buffer (and wipe it clean)
		memset(dateBuff,'\0',sizeBuff);
		memset(timeBuff,'\0',sizeBuff);
		
		serial_print("Welcome to MPX_core main menu, please choose one of the following options by typing the keyword..\n");
		// serial_print("-version\n");
		// serial_print("-help\n");
		// serial_print("-getdate\n");
		// serial_print("-setdate\n");
		// serial_print("-gettime\n");
		// serial_print("-settime\n");
		// serial_print("-clear\n");
		// serial_print("-shutdown\n");
		// possibly adding more commands if needed
		
		sys_req(READ,DEFAULT_DEVICE,buffer,&size);	// goes to polling
		
		
		char *token;				// the word(s) separated from the buffer
		const char split[] = " ";	// the character to indicate splitting of the tokens
		
		token = strtok(buffer,split);	// use strtok() to split the first word from the buffer
		
		if (strcmp(token,"getdate") == 0) {		// for testing purposes
			
			getdate((int*)dateBuff);
			
			serial_print("Current date: ");
			int i = 0;
			while (i < sizeBuff-1) {
				sys_req(WRITE,DEFAULT_DEVICE,(char *)&dateBuff[i],&sizeBuff);
				sys_req(WRITE,DEFAULT_DEVICE,(char *)&dateBuff[i+1],&sizeBuff);
				if (i+2 < sizeBuff-1) serial_print("/");
				i = i+2;
				
			}
			
		}
		
		else if (strcmp(token,"setdate") == 0) { // testing
			
			token = strtok(NULL,split);
			
			if (token == NULL) 
				serial_print("ERROR: invalid number of parameters (too few).\n");
			
			else {
				int yr = atoi(token);
				
				token = strtok(NULL,split);
				
				if (token == NULL) 
					serial_print("ERROR: invalid number of parameters (too few).\n");
				
				else {
					int mth = atoi(token);
					
					token = strtok(NULL,split);
					
					if (token == NULL) 
						serial_print("ERROR: invalid number of parameters (too few).\n");
				
					else {
						int day = atoi(token);
						
						
						if (strtok(NULL,split) != NULL)
							serial_print("ERROR: invalid number of parameters (too many).\n");
						
						else if (yr > 30 || yr < 0 || mth > 12 || mth < 1 || day > 31 || day < 1)
							serial_print("ERROR: invalid date setting.\n");
						
						else {
							setdate(yr,mth,day);
							serial_print("Date adjusted.\n");
						}
					}
				}
			}
		}
		
		
		else if (strcmp(token,"version") == 0) {		// version command
			
			serial_print("version() in progress.. check back later\n");
		}
		
		
		else if (strcmp(token,"help") == 0) {			// help command
			
			serial_print("help() in progress.. check back later\n");
		}
		
		else if (strcmp(token,"gettime") == 0) {		// print current time
			
			get_Time((int*)timeBuff);					// fill time buffer with current time info
			
			serial_print("Current time: ");
			int i = 0;
			while (i < sizeBuff-1) {					// loop the array to print the current time
				sys_req(WRITE,DEFAULT_DEVICE,(char *)&timeBuff[i],&sizeBuff);
				sys_req(WRITE,DEFAULT_DEVICE,(char *)&timeBuff[i+1],&sizeBuff);
				if (i+2 < sizeBuff-1) serial_print(":");
				i = i+2;
				
			}
			
			
		}
		
		else if (strcmp(token,"settime") == 0) {		// change current time
		
			token = strtok(NULL,split);
			
			if (token == NULL) 
				serial_print("ERROR: invalid number of parameters (too few).\n");
			
			else {
				int hr = atoi(token);
				
				token = strtok(NULL,split);
				
				if (token == NULL) 
					serial_print("ERROR: invalid number of parameters (too few).\n");
				
				else {
					int min = atoi(token);
					
					token = strtok(NULL,split);
					
					if (token == NULL) 
						serial_print("ERROR: invalid number of parameters (too few).\n");
				
					else {
						int sec = atoi(token);
						
						
						if (strtok(NULL,split) != NULL)
							serial_print("ERROR: invalid number of parameters (too many).\n");
						
						else if (hr > 23 || hr < 0 || min > 59 || min < 0 || sec > 59 || sec < 0)
							serial_print("ERROR: invalid time setting.\n");
						
						else {
							set_Time(hr,min,sec);
							serial_print("Time adjusted.\n");
						}
						
					}
				}
				
				
				
			}
			
		
		}	// end settime
		
		else if (strcmp(token,"shutdown") == 0) {		// shutdown command
			
			char input;
			int size = 1;
			
			do {
				
				if (input == 'y' || input == 'n') {		// if yes/no will exit
				
					break;
				
				}
				
				serial_print("Do you want to shutdown if yes enter y if no enter n please \n");
				sys_req(READ,DEFAULT_DEVICE,&input,&size);	// goes to polling 
				serial_print("\x1B[2K");	// delete what is printed
				serial_print("\b");
				

			} while (input != 'y' || input != 'n');	// make sure the user enter either y or n
			
			if (input == 'y') {		// if yes will shutdown
					input = ' ';	// reset input
					break;
				
				}
				
			input = ' ';		// reset input
			
		}
		
		
		// else if (strcmp(token,"end") == 0) {	// if user entered "end"
			
			// serial_print("Ending commhand()...\n");	// leave commhand() and begin shutdown
			// break;
			
		// }
		
		else if (strcmp(token,"clear") == 0) {		// clearing the screen
			
			serial_print("\x1B[2J"); // clear screen
			serial_print("\x1B[H"); // send cursor back to (0,0) position (top left corner) before printing main menu
			
		}
		
		else {
			
			serial_print("unknown command entered. Please check spelling and/or syntax..");
			
		}
		
		if (strcmp(token,"clear") != 0) serial_print("\n\n");	// new lines if we didn't clear the screen yet
	
	} // leaving commhand
	
	return;

}
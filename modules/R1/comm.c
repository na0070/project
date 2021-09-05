
#include "../mpx_supt.h"
#include "comm.h"
#include "getsetdate.h"
#include "getandsettime.h"
#include "help.h"
#include "version.h"

#include <stdint.h>
#include <string.h>

#include <core/io.h>
#include <core/serial.h>

#define SIZE 80		//!< The size of the main command buffer
#define SIZEBUFF 7	//!< The size of both time/date buffers

	int size = SIZE; 			
	int sizeBuff = SIZEBUFF;		
	char buffer[SIZE];		//!< Main command buffer 
	int dateBuff[SIZEBUFF];	//!< The date buffer used to print the saved date
	int timeBuff[SIZEBUFF];	//!< The time buffer used to print the saved time
	
	char *token;				//!< Pointer to the first "word" before the space in the main command buffer
	const char split[] = " ";	//!< the character to indicate splitting of the tokens, which is a space
	

/*!
\brief The command handler that handles incoming commands.

Within the Kernel, this function is called, which in return runs an endless loop to continuosly ask the user for commands. To begin,
the function sys_req(READ,DEFAULT_DEVICE,buffer,&size) is called that sends the process to polling(), in which the buffer is filled
with the commands the user types in. Commhand() uses strtok() found in String.h to split the buffer into "tokens", each being a word
separated by a space. The first token is then compared through a series of if-else if statements to determine the appropriate action
to perform.
*/

void commhand() {
	
	while (TRUE) {							// Keep asking for inputs
		memset(buffer,'\0',SIZE);			// set aside memory for the buffer (and wipe it clean)
		memset(dateBuff,'\0',SIZEBUFF);
		memset(timeBuff,'\0',SIZEBUFF);
		
		serial_print("Welcome to MPX_core main menu..\n");
		
		sys_req(READ,DEFAULT_DEVICE,buffer,&size);	// goes to polling
		
		
		
		
		token = strtok(buffer,split);	// use strtok() to split the first word from the buffer
		
		if (strcmp(token,"getdate") == 0) {		// Get current date
			
			getdate((int*)dateBuff);		// call the function and store the data in the date buffer
			
			serial_print("Current date: ");		
			int i = 0;
			while (i < SIZEBUFF-1) {
				sys_req(WRITE,DEFAULT_DEVICE,(char *)&dateBuff[i],&sizeBuff);
				sys_req(WRITE,DEFAULT_DEVICE,(char *)&dateBuff[i+1],&sizeBuff);
				if (i+2 < SIZEBUFF-1) serial_print("/");
				i = i+2;
				
			}
			
		}
		
		else if (strcmp(token,"setdate") == 0) { // Set current date
			
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
			
			version();
		}
		
		
		else if (strcmp(token,"help") == 0) {			// help command
			
			help();
		}
		
		else if (strcmp(token,"gettime") == 0) {		// print current time
			
			get_Time((int*)timeBuff);					// fill time buffer with current time info
			
			serial_print("Current time: ");
			int i = 0;
			while (i < SIZEBUFF-1) {					// loop the array to print the current time
				sys_req(WRITE,DEFAULT_DEVICE,(char *)&timeBuff[i],&sizeBuff);
				sys_req(WRITE,DEFAULT_DEVICE,(char *)&timeBuff[i+1],&sizeBuff);
				if (i+2 < SIZEBUFF-1) serial_print(":");
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
			
			serial_print("Do you want to shutdown if yes enter y if no enter n please \n");
			
			do {
				
				if (input == 'y' || input == 'n') {		// if yes/no will exit
				
					break;
				
				}
				
				
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
	
		
		else if (strcmp(token,"clear") == 0) {		// clearing the screen
			
			serial_print("\x1B[2J"); // clear screen
			serial_print("\x1B[H"); // send cursor back to (0,0) position (top left corner) before printing main menu
			
		}
		
		else {		// an unknown command
			
			serial_print("unknown command entered. Please check spelling and/or syntax..");
			
		}
		
		if (strcmp(token,"clear") != 0) serial_print("\n\n");	// new lines if we didn't clear the screen yet
	
	} // leaving commhand
	
	return;

}
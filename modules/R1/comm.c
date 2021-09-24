
#include "../mpx_supt.h"
#include "comm.h"
#include "getsetdate.h"
#include "getandsettime.h"
#include "help.h"
#include "version.h"
//#include "../R2/structures.c"
#include "../R2/func.c"

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
	
	
	
	pcb *task;
	

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
		
		serial_print("Welcome to MPX_core main menu..\n"); // sys req 
		
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
				serial_print("ERROR: invalid number of parameters (too few).\n");  // sys req 
			
			else {
				int yr = atoi(token);
				
				token = strtok(NULL,split);
				
				if (token == NULL) 
					serial_print("ERROR: invalid number of parameters (too few).\n"); // sys req 
				
				else {
					int mth = atoi(token);
					
					token = strtok(NULL,split);
					
					if (token == NULL) 
						serial_print("ERROR: invalid number of parameters (too few).\n");  // sys req 
				
					else {
						int day = atoi(token);
						
						
						if (strtok(NULL,split) != NULL)
							serial_print("ERROR: invalid number of parameters (too many).\n"); //sys req 
						
						else if (yr > 30 || yr < 0 || mth > 12 || mth < 1 || day > 31 || day < 1)
							serial_print("ERROR: invalid date setting.\n"); // sys req 
						
						else {
							setdate(yr,mth,day);
							serial_print("Date adjusted.\n"); //sys req 
						}
					}
				}
			}
		}
		
		
		else if (strcmp(token,"version") == 0) {						// version command
			
			version();
		}
		
		
		else if (strcmp(token,"help") == 0) {							// help command
			
			help();
		}
		
		else if (strcmp(token,"gettime") == 0) {						// print current time
			
			get_Time((int*)timeBuff);				// fill time buffer with current time info
			
			serial_print("Current time: ");
			int i = 0;
			while (i < SIZEBUFF-1) {					// loop the array to print the current time
				sys_req(WRITE,DEFAULT_DEVICE,(char *)&timeBuff[i],&sizeBuff);
				sys_req(WRITE,DEFAULT_DEVICE,(char *)&timeBuff[i+1],&sizeBuff);
				if (i+2 < SIZEBUFF-1) serial_print(":");
				i = i+2;
				
			}
			
			
		}
		
		else if (strcmp(token,"settime") == 0) {						// change current time
		
			token = strtok(NULL,split);
			
			if (token == NULL) 
				serial_print("ERROR: invalid number of parameters (too few).\n"); // sys req 
			
			else {
				int hr = atoi(token);
				
				token = strtok(NULL,split);
				
				if (token == NULL) 
					serial_print("ERROR: invalid number of parameters (too few).\n"); // sys req 
				
				else {
					int min = atoi(token);
					
					token = strtok(NULL,split);
					
					if (token == NULL) 
						serial_print("ERROR: invalid number of parameters (too few).\n"); // sys req 
				
					else {
						int sec = atoi(token);
						
						
						if (strtok(NULL,split) != NULL)
							serial_print("ERROR: invalid number of parameters (too many).\n");  // sys req 
						
						else if (hr > 23 || hr < 0 || min > 59 || min < 0 || sec > 59 || sec < 0)
							serial_print("ERROR: invalid time setting.\n");  // sys req 
						
						else {
							set_Time(hr,min,sec);
							serial_print("Time adjusted.\n"); // sys req 
						}
						
					}
				}
				
				
				
			}
			
		
		}	// end settime
		
		else if (strcmp(token,"shutdown") == 0) {						// shutdown command
			
			char input;
			int size = 1;
			
			serial_print("Do you want to shutdown if yes enter y if no enter n please \n");  //sys req 
			
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
	
		
		else if (strcmp(token,"clear") == 0) {						// clearing the screen
			
			serial_print("\x1B[2J"); // clear screen
			serial_print("\x1B[H"); // send cursor back to (0,0) position (top left corner) before printing main menu
			
		}
		
		
		//testing
		
		
		
		else if (strcmp(token,"make") == 0) {
			
			
			strcpy(task->name, "PCB #1");
			task->class = 1;
			task->prio = 99;
			task->state = -4;
			memset(task->stack,'\0',1024);
			task->next = NULL;
			
			
			
		}
		
		else if (strcmp(token,"print") == 0) {
			
			char num[3];
			serial_println(task->name);
			
			if (task->class == 0) serial_println("class: system");
			else serial_println("class: application");  //sys req 
			serial_print("Priority: "); serial_println(itoa(task->prio,num));
			serial_print("State: "); serial_println(itoa(task->state,num));
			
			
			
		}
		
		
		//=================================================================================================================================================================================================
		
		
		// R2 commands
		
		
		else if (strcmp(token,"pcb") == 0) {				// all R2 commands will start with keyword "pcb" followed by the actual command word
			
			token = strtok(NULL,split);						// token now is the word after "pcb"
			
			if (token == NULL)								// if nothing is entered after "pcb"
				serial_println("ERROR: no command entered after pcb.");
				
				
			else if (strcmp(token,"suspend") == 0) {		//suspend command
				char* name;
				token = strtok(NULL,split);					// token = <name>
				
				if (token == NULL)
					serial_println("ERROR: too few inputs.");
				
				else {
					strcpy(name,token);						// stored pcb's name
					
					// suspendPCB code here (use "name")
				
				
				
				
				
				
				
				
				
				
				
				
				
				}
				
			}
			
			else if (strcmp(token,"resume") == 0) {			// resume command
				char* name;
				token = strtok(NULL,split);					// token = <name>
				
				if (token == NULL)
					sys_req(WRITE,DEFAULT_DEVICE,"ERROR: too few inputs.\n",24);
				
				else {
					strcpy(name,token);						// stored pcb's name
					
					// resumePCB code here (use "name")
					
					pcb* ptr = findPCB(name);				// find the pcb pointer of the same name and store the pointer for later use
					
					if (ptr == NULL)						// if PCB was not found
						sys_req(WRITE,DEFAULT_DEVICE,"ERROR: could not find PCB.\n",27);
					
					else {
						
						int code = removePCB(ptr);				// store the function's code to check if error or not after remving PCB from its location
					
						if (code == -1)							// error
						sys_req(WRITE,DEFAULT_DEVICE,"ERROR: could not remove PCB.\n",30);
						
						else {
							
							ptr->susp = NOT_SUSPENDED;			// change status of pointer to not suspended
							
							insertPCB(ptr);						// insert the PCB based on its internal info
						}
						
					}
					
				}
			}
			
			else if (strcmp(token,"priority") == 0) {		// set priority command
				char* name;
				int priority;
				token = strtok(NULL,split);					// token = <name>
				
				if (token == NULL)
					serial_println("ERROR: too few inputs.");
				
				else {
					strcpy(name,token);						// stored pcb's name
					
					token = strtok(NULL,split);				// token = <priority>
					
					if (token == NULL)
					serial_println("ERROR: too few inputs.");
				
					else {
						priority = atoi(token);				// stored pcb's priority
						
						
						// set priority code here (use "name", "priority")
						
						if (FindPCB(pntr->name)  != NULL) { // check if the pcb valid 
					if( priority < 0 || priority > 9 ) { // check priority 
					sys_req(WRITE,DEFAULT_DEVICE,"\npriority gotta be between 0 and 9 ", 35);
					
					}
					else {
					
					}
					
					else{
					sys_req(WRITE,DEFAULT_DEVICE,"\nUnvalid PCB",12);
					
					} 
					
						
						
						
						
						
						
						
						
						
						
						
						
						
						
						
						
						
						
						
						
					}
					
					
				}
			}
			
			else if (strcmp(token,"show") == 0) {			// show pcb/all/ready/blocked commands
				char* name;
				token = strtok(NULL,split);					// token = <name>
				
				if (token == NULL)
					serial_println("ERROR: too few inputs.");
				
				else if (strcmp(token,"all")) {
					// "show all" code here
					
					
					
					
					
					
					
					
					
					
				}
				else if (strcmp(token,"ready")) {
					//"show ready" code here
					
					
					
					
					
					
					
					
					
					
					
				}
				else if (strcmp(token,"blocked")) {
					//"show blocked" code here
					
					
					
					
					
				}
				else {
					strcpy(name,token);						// stores pcb's name
					// "show" code here
					
					pcb* ptr = findPCB(name);				// search for the pcb using the name. This will = NULL if it couldn't find it
					
					if (ptr == NULL) {
						
						sys_req(WRITE,DEFAULT_DEVICE,"PCB not found using provided name.\n",36);	// PCB not found
					}
					else {
						sys_req(WRITE,DEFAULT_DEVICE,"\nName: ",8);
						sys_req(WRITE,DEFAULT_DEVICE,ptr->name,20);									// print the name
						
						
						sys_req(WRITE,DEFAULT_DEVICE,"\nClass: ",9);
						
						if (ptr->class == SYSTEM)													// print the class (based on value)
							sys_req(WRITE,DEFAULT_DEVICE,"SYSTEM",7);
						
						else
							sys_req(WRITE,DEFAULT_DEVICE,"APPLICATION",12);
						
						
						sys_req(WRITE,DEFAULT_DEVICE,"\nState: ",9);
						
						if (ptr->state == READY)													// print the state (based on value)
							sys_req(WRITE,DEFAULT_DEVICE,"READY",7);
						
						else
							sys_req(WRITE,DEFAULT_DEVICE,"BLOCKED",8);
						
						
						
						sys_req(WRITE,DEFAULT_DEVICE,"\nStatus: ",10);
						
						if (ptr->susp == NOT_SUSPENDED)													// print the status (based on value)
							sys_req(WRITE,DEFAULT_DEVICE,"NOT SUSPENDED",14);
						
						else
							sys_req(WRITE,DEFAULT_DEVICE,"SUSPENDED",10);
						
						
						
						sys_req(WRITE,DEFAULT_DEVICE,"\nPriority: ",12);
						
						char prioNum[2] = {'\0','\0'};
						itoa(ptr->prio,prioNum);
						
						sys_req(WRITE,DEFAULT_DEVICE,prioNum,strlen(prioNum));							// print the priority (based on value)
						
						
					}
					
				}
				
			}
			
			else if (strcmp(token,"create") == 0) {			// create pcb command -> pcb create <name> <class> <priority>
				char* name;
				int class, priority;
				token = strtok(NULL,split);					// token = <name>
				if (token == NULL)
					serial_println("ERROR: too few inputs.");
				
				else
					strcpy(name,token);						// store pcb's name in pointer
				
				token = strtok(NULL,split);					// token = <class>
				if (token == NULL)
					serial_println("ERROR: too few inputs.");
				
				else 
					class = atoi(token);					// store pcb's class
				
				token = strtok(NULL,split);					// token = <priority>
				if (token == NULL)
					serial_println("ERROR: too few inputs.");
				
				else
					priority = atoi(token);					// store pcb's priority
				
				
				// createPCB code goes here (use "name", "class", "priority")
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
			}
			
			else if (strcmp(token,"delete") == 0) {			// delete pcb command
				char* name;
				token = strtok(NULL,split);					// token = <name>
				
				if (token == NULL)
					serial_println("ERROR: too few inputs.");
				
				else {
					strcpy(name,token);						// stored pcb's name
					
					// deletePCB code here (use "name")
					
					
					
					
					
					
					
					
					
					
					
					
					
					
					
				}
				
				
			}
				
			else if (strcmp(token,"block") == 0) {			// block pcb command
				char* name;
				token = strtok(NULL,split);					// token = <name>
				
				if (token == NULL)
					serial_println("ERROR: too few inputs.");
				
				else {
					strcpy(name,token);						// stored pcb's name
					
					// blockPCB code here (use "name")
				
					if (FindPCB(pntr->name)  != NULL) { // check if the pcb valid 
					removePCB (name); // remove the pcb
					InsertPCB( name);
					
					}
					else{
					sys_req(WRITE,DEFAULT_DEVICE,"\nUnvalid PCB",12);
					
					} 
					
				
				}
			}
			
			else if (strcmp(token,"unblock") == 0) {		// unblock pcb command
				char* name;
				token = strtok(NULL,split);					// token = <name>
				
				if (token == NULL)
					serial_println("ERROR: too few inputs.");
				
				else {
					strcpy(name,token);						// stored pcb's name
					
					// unblockPCB code here (use "name")
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				}
				
			}
			
			// invalid command issued
			else
				serial_println("ERROR: Unknown pcb command entered.");
		}	// end pcb commands
		
		
		
		else {		// an unknown command
			
			serial_print("unknown command entered. Please check spelling and/or syntax.."); //sys req 
			
		}
		
		if (strcmp(token,"clear") != 0) serial_print("\n");	// new lines if we didn't clear the screen yet
	
	} // leaving commhand
	
	return;

}


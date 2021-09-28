
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
	
	print("Welcome to MPX_core..\n",24);
	while (TRUE) {							// Keep asking for inputs
		memset(buffer,'\0',SIZE);			// set aside memory for the buffer (and wipe it clean)
		memset(dateBuff,'\0',SIZEBUFF);
		memset(timeBuff,'\0',SIZEBUFF);
		
		//serial_print("Welcome to MPX_core main menu..\n"); // sys req 
		
		
		sys_req(READ,DEFAULT_DEVICE,buffer,&size);	// goes to polling
		
		
		
		
		token = strtok(buffer,split);	// use strtok() to split the first word from the buffer
		
		if (strcmp(token,"getdate") == 0) {		// Get current date
			
			getdate((int*)dateBuff);		// call the function and store the data in the date buffer
			
			// serial_print("Current date: ");
			print("Current date: ", 15);
			int i = 0;
			while (i < SIZEBUFF-1) {
				sys_req(WRITE,DEFAULT_DEVICE,(char *)&dateBuff[i],&sizeBuff);
				sys_req(WRITE,DEFAULT_DEVICE,(char *)&dateBuff[i+1],&sizeBuff);
				if (i+2 < SIZEBUFF-1) print("/",1);
				i = i+2;
				
			}
			
		}
		
		else if (strcmp(token,"setdate") == 0) { // Set current date
			
			token = strtok(NULL,split);
			
			if (token == NULL) 
				print("ERROR: invalid number of parameters (too few).\n",48);  // sys req 
			
			else {
				int yr = atoi(token);
				
				token = strtok(NULL,split);
				
				if (token == NULL) 
					print("ERROR: invalid number of parameters (too few).\n",48); // sys req 
				
				else {
					int mth = atoi(token);
					
					token = strtok(NULL,split);
					
					if (token == NULL) 
						print("ERROR: invalid number of parameters (too few).\n",48);  // sys req 
				
					else {
						int day = atoi(token);
						
						
						if (strtok(NULL,split) != NULL)
							print("ERROR: invalid number of parameters (too many).\n",49); //sys req 
						
						else if (yr > 30 || yr < 0 || mth > 12 || mth < 1 || day > 31 || day < 1)
							print("ERROR: invalid date setting.\n",30); // sys req 
						
						else {
							setdate(yr,mth,day);
							print("Date adjusted.\n",16); //sys req 
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
			
			print("Current time: ",17);
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
				print("ERROR: invalid number of parameters (too few).\n",48); // sys req 
			
			else {
				int hr = atoi(token);
				
				token = strtok(NULL,split);
				
				if (token == NULL) 
					print("ERROR: invalid number of parameters (too few).\n",48); // sys req 
				
				else {
					int min = atoi(token);
					
					token = strtok(NULL,split);
					
					if (token == NULL) 
						print("ERROR: invalid number of parameters (too few).\n",48); // sys req 
				
					else {
						int sec = atoi(token);
						
						
						if (strtok(NULL,split) != NULL)
							print("ERROR: invalid number of parameters (too many).\n",49);  // sys req 
						
						else if (hr > 23 || hr < 0 || min > 59 || min < 0 || sec > 59 || sec < 0)
							print("ERROR: invalid time setting.\n",30);  // sys req 
						
						else {
							set_Time(hr,min,sec);
							print("Time adjusted.\n",16); // sys req 
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
				print("ERROR: no command entered after pcb.\n",40);
				
				
			else if (strcmp(token,"suspend") == 0) {		//suspend command
				char* name = " ";							// initialize a name
				token = strtok(NULL,split);					// token = <name>
				
				if (token == NULL)
					serial_println("ERROR: too few inputs.");
				
				else {
					strcpy(name,token);						// stored pcb's name
					
					// suspendPCB code here (use "name")
				
					pcb* ptr = findPCB(name);				// find the pcb pointer of the same name and store the pointer for later use
					
					if (ptr == NULL)						// if PCB was not found
						print("ERROR: could not find PCB.\n",27);
					else {
						
						int code = removePCB(ptr);				// store the function's code to check if error or not after remving PCB from its location
					
						if (code == -1)							// error
						print("ERROR: could not remove PCB.\n",30);
						
						else {
							
							ptr->susp = SUSPENDED;				// change status of pointer to suspended
							
							insertPCB(ptr);						// insert the PCB based on its internal info
						}
						
					}
					
					
				}
				
			}
			
			else if (strcmp(token,"resume") == 0) {			// resume command
				char* name = " ";
				token = strtok(NULL,split);					// token = <name>
				
				if (token == NULL)
					print("ERROR: too few inputs.\n",24);
				
				else {
					strcpy(name,token);						// stored pcb's name
					
					// resumePCB code here (use "name")
					
					pcb* ptr = findPCB(name);				// find the pcb pointer of the same name and store the pointer for later use
					
					if (ptr == NULL)						// if PCB was not found
						print("ERROR: could not find PCB.\n",27);
					
					else {
						
						int code = removePCB(ptr);				// store the function's code to check if error or not after remving PCB from its location
					
						if (code == -1)							// error
						print("ERROR: could not remove PCB.\n",30);
						
						else {
							
							ptr->susp = NOT_SUSPENDED;			// change status of pointer to not suspended
							
							insertPCB(ptr);						// insert the PCB based on its internal info
						}
						
					}
					
				}
			}
			
			else if (strcmp(token,"priority") == 0) {		// set priority command
				char* name = " ";
				int priority;
				token = strtok(NULL,split);					// token = <name>
				
				if (token == NULL)
					print("ERROR: too few inputs.",23);
				
				else {
					strcpy(name,token);						// stored pcb's name
					
					token = strtok(NULL,split);				// token = <priority>
					
					if (token == NULL)
						print("ERROR: too few inputs.",23);
				
					else {
						priority = atoi(token);				// stored pcb's priority
						
						
						// set priority code here (use "name", "priority")
						pcb* pointer2 = findPCB(name);
						if (findPCB(name)  != NULL) { // check if the pcb valid 
							if( priority < 0 || priority > 9 )  // check priority 
								print("\npriority gotta be between 0 and 9\n", 36);
					
					
							else {
						
							removePCB(pointer2); // remove the pcb
							pointer2->prio= priority ; // change the status of the PCB
							insertPCB(pointer2); // insert it again 
						
							}
							
						}
					
						else
							print("\nUnvalid PCB",12);
						
					}
					
					
				}
			}
			
			else if (strcmp(token,"show") == 0) {			// show pcb/all/ready/blocked commands
				char* name = " ";
				token = strtok(NULL,split);					// token = <name>
				
				if (token == NULL)
					print("ERROR: too few inputs.\n",24);
				
				else if (strcmp(token,"all") == 0) {
					showqueue(ready);				//prints information of the entire ready queue
					showqueue(sus_ready);			//prints information of the suspended ready queue
					showqueue(blocked);				//prints information of the entire blocked queue
					showqueue(sus_blocked);			//prints information of the suspended blocked queue
					
					
					
					
					
					
					
					
					
					
				}
				else if (strcmp(token,"ready") == 0) {
					//"show ready" code here
					showqueue(ready);				//prints information of the entire ready queue
					showqueue(sus_ready);			//prints information of the suspended ready queue
					
					
					
					
					
					
					
					
					
					
				}
				else if (strcmp(token,"blocked") == 0) {
					//"show blocked" code here
					showqueue(blocked);				//prints information of the entire blocked queue
					showqueue(sus_blocked);			//prints information of the suspended blocked queue
					
					
					
					
				}
				else {
					strcpy(name,token);						// stores pcb's name
					// "show" code here
					
					pcb* ptr = findPCB(name);				// search for the pcb using the name. This will = NULL if it couldn't find it

					if (ptr == NULL) {
						
						print("PCB not found using provided name.\n",36);	// PCB not found
					}
					else {
						print("found pcb, will show/n",23);
						showPCB(ptr);						// call the function to print all the PCB's information
						
					}
					
				}
				
			}
			
			else if (strcmp(token,"create") == 0) {			// create pcb command -> pcb create <name> <class> <priority>
				char* name = " ";
				int class, priority;
				token = strtok(NULL,split);					// token = <name>
				if (token == NULL)
					serial_println("ERROR: too few inputs.");
				
				else {
					strcpy(name,token);						// store pcb's name in pointer
				
					token = strtok(NULL,split);					// token = <class>
					if (token == NULL)
						serial_println("ERROR: too few inputs.");
					
					else {
						class = atoi(token);					// store pcb's class
					
						token = strtok(NULL,split);					// token = <priority>
						if (token == NULL)
							serial_println("ERROR: too few inputs.");
						
						else {
							priority = atoi(token);					// store pcb's priority
						
						
							// createPCB code goes here (use "name", "class", "priority")
							
							if (findPCB(name) != NULL) 																// Checking if the name used or not 
								serial_println("Name already used.");
								
							else { 
								print("can make new pcb\n",20);			
								if (class != SYSTEM && class != APPLICATION) {			// Checking if the class is valid 
									serial_println("Class not valid.");
									
								} else{
									
									if (priority < 0 || priority > 9 ) {											// Checking if the priority number is valid 
										
										serial_println("Priority not valid.");
									} else {
										
										pcb* pntr = setupPCB(name, class, priority); 				// assigning the SetupPCB result to pointer
										
										if (pntr == NULL) {													    	// Checking if the PCB created succsefully  
											
											serial_println("Couldn't setup PCB.");
										} else {
											print("setUp successful\n",19);
											insertPCB(pntr);															// inserting the new PCB in the right place 			
										}
									}
								}
								
							}
						}
					}// up to here
				}
				
			}
			
			else if (strcmp(token,"delete") == 0) {			// delete pcb command
				char* name = " ";
				token = strtok(NULL,split);					// token = <name>
				
				if (token == NULL)
					serial_println("ERROR: too few inputs.");
				
				else {
					strcpy(name,token);						// stored pcb's name
					
					// deletePCB code here (use "name")
					
					pcb* pntr = findPCB(name);												// Assigning findPCB results topointer 
					
					if (pntr == NULL) {																// Checking if the PCB is unavalibale 
						
						serial_println("PCB couldn't be found");
						
					} else {
						
						int error_Check = removePCB(pntr);								// Removing the PCB and saves the error code
						
						if (error_Check == -1) {
							
							serial_println("PCB couldn't be removed");
							
						} else { 
							
							freePCB(pntr);															// Delete PCB 
							serial_println("PCB has been deleted succesfully!");
						}
					}
	
				}
				
				
			}
				
			else if (strcmp(token,"block") == 0) {			// block pcb command
				char* name = " ";
				token = strtok(NULL,split);					// token = <name>
				
				if (token == NULL)
					serial_println("ERROR: too few inputs.");
				
				else {
					strcpy(name,token);						// stored pcb's name
					
					// blockPCB code here (use "name")
				
				        pcb* pointer = findPCB(name);
					if (findPCB(name)  != NULL) { // check if the pcb valid 
					removePCB (pointer); // remove the pcb
					pointer->state = BLOCKED; // change the status of the PCB
					insertPCB(pointer); // insert it again 
					
					}
					else{
					print("\nUnvalid PCB\n",13); // error massage
					
					} 
					
				
				}
			}
			
			else if (strcmp(token,"unblock") == 0) {		// unblock pcb command
				char* name = " ";
				token = strtok(NULL,split);					// token = <name>
				
				if (token == NULL)
					serial_println("ERROR: too few inputs.");
				
				else {
					strcpy(name,token);						// stored pcb's name
					
					// unblockPCB code here (use "name")
				
				pcb* pointer1 = findPCB(name);
					if (findPCB(name)  != NULL) { // check if the pcb valid 
					removePCB (pointer1); // remove the pcb
					pointer1->state = READY; // change the status of the PCB
					insertPCB(pointer1); // insert it again 
					
					}
					else{
					print("\nUnvalid PCB\n",13); // error massage
					
					} 
				
				
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


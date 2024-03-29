#include "../mpx_supt.h"
#include "comm.h"
#include "getsetdate.h"
#include "getandsettime.h"
#include "help.h"
#include "version.h"
#include "../R2/func.h"
#include "../procsr3.h"
#include "../R4/Alarm.h"
#include "../R5/func5.h"


#include <stdint.h>
#include <string.h>

#include <core/io.h>
#include <core/serial.h>

#define SIZE 50		//!< The size of the main command buffer
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

	
	print("Welcome to MPX_core..\n",23);

	while (TRUE) {							// Keep asking for inputs
		memset(buffer,'\0',SIZE);			// set aside memory for the buffer (and wipe it clean)
		memset(dateBuff,'\0',SIZEBUFF);
		memset(timeBuff,'\0',SIZEBUFF);

		print("$ ", 3);

		size = SIZE;		// reset size before sys_req READ
		sys_req(READ,DEFAULT_DEVICE,buffer,&size);	// goes to polling

		token = strtok(buffer,split);	// use strtok() to split the first word from the buffer
		
		// beginning of commands


		if (strcmp(token,"getdate") == 0) {		// Get current date
			
			getdate((int*)dateBuff);		// call the function and store the data in the date buffer
			
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
				print("[31mERROR: invalid number of parameters (too few).\x1B[39m\n",48);  // sys req 
			
			else {
				int yr = atoi(token);
				
				token = strtok(NULL,split);
				
				if (token == NULL) 
					print("[31mERROR: invalid number of parameters (too few).\x1B[39m\n",48); // sys req 
				
				else {
					int mth = atoi(token);
					
					token = strtok(NULL,split);
					
					if (token == NULL) 
						print("[31mERROR: invalid number of parameters (too few).\x1B[39m\n",48);  // sys req 
				
					else {
						int day = atoi(token);
						
						
						if (strtok(NULL,split) != NULL)
							print("[31mERROR: invalid number of parameters (too many).\x1B[39m\n",49); //sys req 
						
						else if (yr > 30 || yr < 0 || mth > 12 || mth < 1 || day > 31 || day < 1)
							print("[31mERROR: invalid date setting.\x1B[39m\n",30); // sys req 
						
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
				if (i+2 < SIZEBUFF-1) print(":",1);
				i = i+2;
				
			}
			
			
		}
		
		else if (strcmp(token,"settime") == 0) {						// change current time
		
			token = strtok(NULL,split);
			
			if (token == NULL) 
				print("[31mERROR: invalid number of parameters (too few).\x1B[39m\n",48); // sys req 
			
			else {
				int hr = atoi(token);
				
				token = strtok(NULL,split);
				
				if (token == NULL) 
					print("[31mERROR: invalid number of parameters (too few).\x1B[39m\n",48); // sys req 
				
				else {
					int min = atoi(token);
					
					token = strtok(NULL,split);
					
					if (token == NULL) 
						print("[31mERROR: invalid number of parameters (too few).\x1B[39m\n",48); // sys req 
				
					else {
						int sec = atoi(token);
						
						
						if (strtok(NULL,split) != NULL)
							print("[31mERROR: invalid number of parameters (too many).\x1B[39m\n",49);  // sys req 
						
						else if (hr > 23 || hr < 0 || min > 59 || min < 0 || sec > 59 || sec < 0)
							print("[31mERROR: invalid time setting.\x1B[39m\n",30);  // sys req 
						
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
			
			print("Do you want to shutdown if yes enter y if no enter n please \n",61);  //sys req 
			
			while (1) {

				sys_req(READ,DEFAULT_DEVICE,&input,&size);	// goes to polling 
				print("\x1B[2K",4);	// delete what is printed
				print("\b",1);
				
				if (input == 'y' || input == 'n') {		// if yes/no will exit
				
					break;
				
				}
				
				

			} 
			//while (input != 'y' || input != 'n');	// make sure the user enter either y or n
			
			if (input == 'y') {		// if yes will shutdown
					input = ' ';	// reset input
					//break;
					pcb* end = findPCB("idle_process");
					removePCB(end);

					end = findPCB("alarm_process");
					removePCB(end);

					end = findPCB("infinite_process");
					removePCB(end);
					sys_req(EXIT, DEFAULT_DEVICE, NULL, NULL);
				
				}
				
			input = ' ';		// reset input
			
		}
	
		
		else if (strcmp(token,"clear") == 0) {						// clearing the screen
			
			print("\x1B[2J",5); // clear screen
			print("\x1B[H",4); // send cursor back to (0,0) position (top left corner) before printing main menu
			
		}
		
		//=================================================================================================================================================================================================
		
		
		// R2 commands
		
		
		else if (strcmp(token,"pcb") == 0) {				// all R2 commands will start with keyword "pcb" followed by the actual command word
			
			token = strtok(NULL,split);						// token now is the word after "pcb"
			
			if (token == NULL)								// if nothing is entered after "pcb"
				print("[31mERROR: no command entered after pcb.\x1B[39m\n",48);
				
				
			else if (strcmp(token,"suspend") == 0) {		//suspend command
				char* name = " ";							// initialize a name
				token = strtok(NULL,split);					// token = <name>
				
				if (token == NULL)
					print("[31mERROR: too few inputs.\x1B[39m\n",25);
				
				else {
					strcpy(name,token);						// stored pcb's name
					
					if (strcmp(name,"command_handler") == 0)
						print("[31mERROR: cannot suspend this process\x1B[39m\n",37);
						
					else if(strcmp(name,"idle_process") == 0)
						print("[31mERROR: cannot suspend this process\x1B[39m\n",37);

					else if (strcmp(name,"alarm_process") == 0)
						print("[31mERROR: cannot suspend this process\x1B[39m\n",37);

					else {
						// suspendPCB code here (use "name")
					
						pcb* ptr = findPCB(name);				// find the pcb pointer of the same name and store the pointer for later use
						
						if (ptr == NULL)						// if PCB was not found
							print("[31mERROR: could not find PCB.\x1B[39m\n",27);
						else {
							
							int code = removePCB(ptr);				// store the function's code to check if error or not after remving PCB from its location
						
							if (code == -1)							// error
							print("[31mERROR: could not remove PCB.\x1B[39m\n",30);
							
							else {
								
								ptr->susp = SUSPENDED;				// change status of pointer to suspended
								
								insertPCB(ptr);						// insert the PCB based on its internal info
							}
							
						}

					}
					
				}
				
			}
			
			else if (strcmp(token,"resume") == 0) {			// resume command
				char* name = " ";
				token = strtok(NULL,split);					// token = <name>
				
				if (token == NULL)
					print("[31mERROR: too few inputs.\x1B[39m\n",33);
				
				else {
					strcpy(name,token);						// stored pcb's name
					
					// resumePCB code here (use "name")
					
					pcb* ptr = findPCB(name);				// find the pcb pointer of the same name and store the pointer for later use
					
					if (ptr == NULL)						// if PCB was not found
						print("[31mERROR: could not find PCB.\x1B[39m\n",38);
					
					else {
						
						int code = removePCB(ptr);				// store the function's code to check if error or not after remving PCB from its location
					
						if (code == -1)							// error
						print("[31mERROR: could not remove PCB.\x1B[39m\n",36);
						
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
					print("[31mERROR: too few inputs.\x1B[39m",33);
				
				else {
					strcpy(name,token);						// stored pcb's name
					
					token = strtok(NULL,split);				// token = <priority>
					
					if (token == NULL)
						print("[31mERROR: too few inputs.\x1B[39m",33);
				
					else {
						priority = atoi(token);				// stored pcb's priority
						
						//if (strcmp(token,"command_handler") == 0)

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
							print("\nInvalid PCB",13);
						
					}
					
					
				}
			}
			
			else if (strcmp(token,"show") == 0) {			// show pcb/all/ready/blocked commands
				char* name = " ";
				token = strtok(NULL,split);					// token = <name>
				
				if (token == NULL)
					print("[31mERROR: too few inputs.\x1B[39m\n",34);
				
				else if (strcmp(token,"all") == 0) {
					
					print("\nREADY QUEUE:",14);
					showqueue("ready");				//prints information of the entire ready queue
					
					print("\n\nSUSPENDED READY QUEUE:",25);
					showqueue("sus_ready");			//prints information of the suspended ready queue
					
					print("\n\nBLOCKED QUEUE:",17);
					showqueue("blocked");				//prints information of the entire blocked queue
					
					print("\n\nSUSPENDED BLOCKED QUEUE:",27);
					showqueue("sus_blocked");			//prints information of the suspended blocked queue
					
					
				}
				else if (strcmp(token,"ready") == 0) {
					//"show ready" code here
					print("\nREADY QUEUE:",16);
					showqueue("ready");				//prints information of the entire ready queue
					
					print("\n\nSUSPENDED READY QUEUE:",26);
					showqueue("sus_ready");			//prints information of the suspended ready queue
					
					
					
				}
				else if (strcmp(token,"blocked") == 0) {
					//"show blocked" code here
					print("\n\nBLOCKED QUEUE:",17);
					showqueue("blocked");				//prints information of the entire blocked queue
					
					print("\n\nSUSPENDED BLOCKED QUEUE:",27);
					showqueue("sus_blocked");			//prints information of the suspended blocked queue
					
					
				}
				else {
					strcpy(name,token);						// stores pcb's name
					// "show PCB" code here
					
					pcb* ptr = findPCB(name);				// search for the pcb using the name. This will = NULL if it couldn't find it

					if (ptr == NULL) {
						
						print("PCB not found using provided name.\n",36);	// PCB not found
					}
					else {
						showPCB(ptr);						// call the function to print all the PCB's information
						
					}
					
				}
				
			}
			
			// else if (strcmp(token,"create") == 0) {			// create pcb command -> pcb create <name> <class> <priority>
			// 	char* name = " ";
			// 	int class, priority;
			// 	token = strtok(NULL,split);					// token = <name>
			// 	if (token == NULL)
			// 		print("ERROR: too few inputs.\n",25);
				
			// 	else {
			// 		strcpy(name,token);						// store pcb's name in pointer
				
			// 		token = strtok(NULL,split);					// token = <class>
			// 		if (token == NULL)
			// 			print("ERROR: too few inputs.\n",25);
					
			// 		else {
			// 			class = atoi(token);					// store pcb's class
					
			// 			token = strtok(NULL,split);					// token = <priority>
			// 			if (token == NULL)
			// 				print("ERROR: too few inputs.\n",25);
						
			// 			else {
			// 				priority = atoi(token);					// store pcb's priority
						
						
			// 				// createPCB code goes here (use "name", "class", "priority")
							
			// 				if (findPCB(name) != NULL) 																// Checking if the name used or not 
			// 					print("Name already used.\n",21);
								
			// 				else { 			
			// 					if (class != SYSTEM && class != APPLICATION) {			// Checking if the class is valid 
			// 						print("Class not valid.\n",17);
									
			// 					} else{
									
			// 						if (priority < 0 || priority > 9 ) {											// Checking if the priority number is valid 
										
			// 							print("Priority not valid.\n",22);
			// 						} else {
										
			// 							pcb* pntr = setupPCB(name, class, priority); 				// assigning the SetupPCB result to pointer
										
			// 							if (pntr == NULL) {													    	// Checking if the PCB created succsefully  
											
			// 								print("Couldn't setup PCB.\n",22);
			// 							} else {
											
			// 								insertPCB(pntr);															// inserting the new PCB in the right place 	
			// 								print("setUp successful\n",19);		
			// 							}
			// 						}
			// 					}
								
			// 				}
			// 			}
			// 		}// up to here
			// 	}
				
			// }
			
			else if (strcmp(token,"delete") == 0) {			// delete pcb command
				char* name = " ";
				token = strtok(NULL,split);					// token = <name>
				
				if (token == NULL)
					print("ERROR: too few inputs.\x1B[39m\n",29);
				
				else {
					strcpy(name,token);						// stored pcb's name
					
					// deletePCB code here (use "name")
					if (strcmp(name,"command_handler") == 0)
						print("[31mERROR: cannot remove this process\x1B[39m\n",55);
						
					else if(strcmp(name,"idle_process") == 0)
						print("[31mERROR: cannot remove this process\x1B[39m\n",55);

					else if (strcmp(name,"alarm_process") == 0)
						print("[31mERROR: cannot remove this process\x1B[39m\n",55);

					else {

						pcb* pntr = findPCB(name);												// Assigning findPCB results topointer 

						
						if (pntr == NULL) {																// Checking if the PCB is unavalibale 
							
							print("[31mERROR: PCB couldn't be found.\x1B[39m\n",41);
							
						} 

						else if (strcmp(name,"infinite_process") == 0 && pntr->susp == NOT_SUSPENDED)
							print("[31mERROR: Trying to remove infinite process requires it to be suspended first\x1B[39m\n",86);

						else {
							
							int error_Check = removePCB(pntr);								// Removing the PCB and saves the error code
							
							if (error_Check == -1) {
								
								print("[31mERROR: PCB couldn't be removed (pcb may not be found)\x1B[39m\n",65);
								
							} else { 
								
								freePCB(pntr);															// Delete PCB 
								print("PCB has been deleted succesfully!\n",35);
							}
						}

					}
	
				}
				
				
			}
				
			else if (strcmp(token,"block") == 0) {			// block pcb command
				char* name = " ";
				token = strtok(NULL,split);					// token = <name>
				
				if (token == NULL)
					print("[31mERROR: too few inputs.\x1B[39m\n",34);
				
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
					print("\nInvalid PCB\x1B[39m\n",20); // error massage
					
					} 
					
				
				}
			}
			
			else if (strcmp(token,"unblock") == 0) {		// unblock pcb command
				char* name = " ";
				token = strtok(NULL,split);					// token = <name>
				
				if (token == NULL)
					print("[31mERROR: too few inputs.\x1B[39m\n",34);
				
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
					print("\nInvalid PCB\x1B[39m\n",20); // error massage
					
					} 
				
				
				}
				
			}


			// invalid command issued
			else
				print("[31mERROR: Unknown pcb command entered.\x1B[39m",46);
		}	// end pcb commands
		
		//=================================================================================================================================
						//R3/4 commands

		// not used anymore
		// else if (strcmp(token,"yield") == 0) {

		// 	// yield code here

		// 	// asm volatile ("int $60");
		// 	print("yield\n", 6);


		// }

		else if (strcmp(token,"loadr3") == 0) {



			// loadr3 code here

			loadr_pcb("proc1", APPLICATION, SUSPENDED, 5, (u32int)proc1 );
			loadr_pcb("proc2", APPLICATION, SUSPENDED, 5, (u32int)proc2 );
			loadr_pcb("proc3", APPLICATION, SUSPENDED, 5, (u32int)proc3 );
			loadr_pcb("proc4", APPLICATION, SUSPENDED, 5, (u32int)proc4 );
			loadr_pcb("proc5", APPLICATION, SUSPENDED, 5, (u32int)proc5 );
		}

		else if (strcmp(token,"alarm") == 0) {

			token = strtok(NULL,split);
			if (token == NULL)
				print("[31mERROR: too few inputs\x1B[39m\n",33);

			else {

				char* message = token;

				token = strtok(NULL,split);

				if (token == NULL)
					print("[31mERROR: too few inputs\x1B[39m\n",33);

				else {
					int time = atoi(token);

					createAlarm(message, time);
				}

			}


		}

		//=================================================================================================================================
						//R5 commands

		else if (strcmp(token,"heap") == 0) {		// heap commands

			// u32int testSize = 50000;

			token = strtok(NULL,split);						// token now is the word after "heap"
			
			if (token == NULL)								// if nothing is entered after "heap"
				print("[31mERROR: no command entered after heap.\x1B[39m\n",49);



			else if (strcmp(token,"check") == 0) {			// check if heap is empty or not, "isEmpty"
				
				if (isEmpty()) {
					print("Heap is empty\n",15);
					print("\n",2);
				}
				else {
					print("Heap is NOT empty\n",19);
					print("\n",2);
				}

			}

			else if (strcmp(token,"show") == 0) {				// show free or allocated

				token = strtok(NULL,split);						// token is now word after "show" (either free or allocated)

				if (token == NULL)
					print("[31mERROR: too few inputs.\x1B[39m\n",34);

				else if (strcmp(token,"free") == 0) {

					// show free memory here
					print("free\n",6);
					showList(FREE);

				}

				else if (strcmp(token,"allocated") == 0) {

					// show allocated memory here

					showList(ALLOCATED);

				}

				else												// neither free nor allocated (invalid)
					print("[31mERROR: invalid input entered.\x1B[39m\n",41);



			}

			else
				print("[31mERROR: Unknown heap command\x1B[39m\n",39);

		}	// end of heap commands



			else if (strcmp(token,"logo") == 0)
				logo();


		else {		// an unknown command
			
			
			print("\x1B[91munknown command entered. Please check spelling and/or syntax..\x1B[39m",73);
			
		}
		
		if (strcmp(token,"clear") != 0) print("\n",2);	// new lines if we didn't clear the screen yet
	

		//sys_req(IDLE, DEFAULT_DEVICE, NULL, NULL);

	} // leaving commhand


	

}


// function to print custom logo
void logo() {

print("welcome to MPX of group\n",25);
  print("\x1B[31m",6);
	          print("                1111 \n",23);
              print("                1111                                                                  1 \n",90);
              print("                1111                                                                  111 \n",92);
              print("                1111                                                                  11111 \n",94);
              print("                1111111111111111111111111111111111111111111111111111111111111111111111111111 \n",95);
              print("                111111111111111111111111111111111111111111111111111111111111111111111111111111 \n",97);
              print("                11111111111111111111111111111111111111111111111111111111111111111111111111111111 \n",99);
              print("                1111111111111111111111111111111111111111111111111111111111111111111111111111111111 \n",101);
              print("                11111111111111111111111111111111111111111111111111111111111111111111111111111111111 \n",102);
              print("                1111111111111111111111111111111111111111111111111111111111111111111111111111111111111 \n",104);
              print("                111111111111111111111111111111111111111111111111111111111111111111111111111111111111111 \n",106);
              print("                11111111111111111111111111111111111111111111111111111111111111111111111111111111111111111 \n",108);
              print("                1111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111\n",109);
              print("                1111 \n",23);
              print("                1111 \n",23);
              print("                1111 \n",23);
              print("                1111 \n",23);
              print("\x1B[32m",6);
              
            print("                                            666666666666666666666666666666666666666 \n",86);
            print("                                      6666666666666666666666666666666666666666666666666 \n",90);
            print("                                   666666666666666666666666666666666666666666666666666666666 \n",95);
            print("                                666666666666666666666666666666666666666666666666666666666666666 \n",98);
            print("                              6666666666666666666666666666666666666666666666666666666666666666666 \n",100);
            print("                            66666666666666666666666666666666666666666666666666666666666666666666666 \n",102);
            print("                          666666666666666666666666666666666666666666666666666666666666666666666666666 \n",104);
            print("                         66666666666666666666666666666666666666666666666666666666666666666666666666666 \n",105);
            print("                       666666666666666666666666666666666666666666666666666666666666666666666666666666666 \n",107);
            print("                      6666666666666666                        66666                      6666666666666666 \n",108);
            print("                     66666666666                                66666                          66666666666 \n",109);
            print("                    666666666                                    66666                            666666666 \n",110);
            print("                    6666666                                        6666                             6666666 \n",110);
            print("                   666666                                           6666                              666666 \n",111);
            print("                   66666                                             6666                              66666 \n",111);
            print("                  66666                                               6666                              66666 \n",112);
            print("                  6666                                                6666                              66666 \n",112);
            print("                  6666                                                66666                              6666 \n",112);
            print("                  6666                                                66666                              6666 \n",112);
            print("                  6666                                                66666                              6666 \n",112);
            print("                  66666                                               66666                              6666 \n",112);
            print("                   66666                                             666666                              6666 \n",112);
            print("                   666666                                           6666666                              6666 \n",112);
            print("                    6666666                                        66666666                              6666 \n",112);
            print("                    666666666                                    666666666                              66666 \n",112);
            print("                     66666666666                               66666666666                              6666 \n",111);
            print("                      6666666666666666                   6666666666666666                              66666 \n",111);
            print("                       6666666666666666666666666666666666666666666666666                              66666 \n",110);
            print("                         6666666666666666666666666666666666666666666666                              66666 \n",109);
            print("                          66666666666666666666666666666666666666666666                              666666 \n",109);
            print("                            66666666666666666666666666666666666666666                               66666 \n",108);
            print("                              6666666666666666666666666666666666666                                  666 \n",107);
            print("                                666666666666666666666666666666666                                     6 \n",106);
            print("                                   666666666666666666666666666 \n",65);
            print("                                       66666666666666666 \n",59);
            print("\x1B[39m",6);


}
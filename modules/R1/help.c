#include "help.h"
#include "../mpx_supt.h"

#include <stdint.h>
#include <string.h>
#include <core/serial.h>


/*!
\brief prints several statements to explain what commands can be implemeted and the syntax for some of them
@param none
*/
void help(){
	print("\x1B[93m",6);
print("commands: [version] [settime] [gettime] [setdate] [getdate] [shutdown] [delete PCB] [block PCB] [unblock PCB] \n[suspend PCB] [resume PCB] [set PCB priority] [show PCB] [show ready] [show blocked] [show all] [clear] [help] \n",224);
print("[loadr3] [alarm] [show free memory] [show allocated memory] [check]\n",69);
  
print("setdate [setdate year month day] \n",35);
print("settime [settime hour minute second] \n",39);
//print("pcb create (name) (class) (#priority) \n",39);
print("pcb delete (name) \n",20);
print("pcb block (name) \n",19);
print("pcb unblock (name) \n",21);
print("pcb suspend (name) \n",21);
print("pcb resume (name) \n",20);
print("pcb priority (name) (#new priority) \n",38);
print("pcb show (name) \n",18);
print("pcb show ready \n",17);
print("pcb show blocked \n",19);
print("pcb show all \n",15);
print("loadr3 \n",9);
print("alarm (message) (delay) \n",26);
print("heap show allocated \n",22);
print("heap show free \n",17);
print("heap check \n",13);

print("\x1B[39m",6);


}

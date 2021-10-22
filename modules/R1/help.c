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
print("commands: [version] [settime] [gettime] [setdate] [getdate] [shutdown] [delete PCB] [block PCB] [unblock PCB] \n[suspend PCB] [resume PCB] [set PCB priority] [show PCB] [show ready] [show blocked] [show all] [clear] [help] \n",151);
print("[loadr3] [alarm] \n",20);
  
print("setdate [setdate year month day] \n",35);
print("settime [settime hour minute second] \n",39);
//print("pcb create (name) (class) (#priority) \n",39);
print("pcb delete (name) \n",20);
print("pcb block (name) \n",19);
print("pcb unblock (name) \n",21);
print("pcb suspend (name) \n",21);
print("pcb resume (name) \n",20);
print("pcb priority (name) (#new priority) \n",37);
print("pcb show (name) \n",19);
print("pcb show ready \n",19);
print("pcb show blocked \n",19);
print("pcb show all \n",15);
print("loadr3 \n",9);
print("alarm (message) (delay) \n",26);



}

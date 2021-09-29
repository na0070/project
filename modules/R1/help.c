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
print("commands: [version] [settime] [gettime] [setdate] [getdate] [shutdown] [clear] [help] \n",88);
print("setdate [setdate year month day] \n",35);
print("settime [settime hour minute second] \n",39);



}

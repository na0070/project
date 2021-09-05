#include "help.h"

#include <stdint.h>
#include <string.h>
#include <core/serial.h>

/*!
\brief prints several statements to explain what commands can be implemeted and the syntax for some of them
@param none
*/
void help(){
serial_print("commands: [version] [settime] [gettime] [setdate] [getdate] [shutdown] [clear] [help] \n");

serial_print("setdate [setdate year month day] \n");
serial_print("settime [settime hour minute second] \n");


}

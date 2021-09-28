#include "help.h"

#include <stdint.h>
#include <string.h>
#include <core/serial.h>

/*!
\brief prints several statements to explain what commands can be implemeted and the syntax for some of them
@param none
*/
void help(){
sys_req(WRITE,DEFAULT_DEVICE,"commands: [version] [settime] [gettime] [setdate] [getdate] [shutdown] [clear] [help] \n",87);
sys_req(WRITE,DEFAULT_DEVICE,"setdate [setdate year month day] \n",34);
sys_req(WRITE,DEFAULT_DEVICE,"settime [settime hour minute second] \n",37);



}

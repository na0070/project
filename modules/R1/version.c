
#include "version.h"

#include <stdint.h>
#include <string.h>
#include <core/serial.h>

/*!
\brief Prints a statement indicating the current version and date of completion
@param none
*/
void version(){
sys_req(WRITE,DEFAULT_DEVICE," R2 published 09/26/2021 \n",27);

}

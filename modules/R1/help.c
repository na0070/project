#include "help.h"

#include <stdint.h>
#include <string.h>
#include <core/serial.h>


void help(){
serial_print("commands: [version] [settime] [gettime] [setdate] [getdate] [shutdown] [clear] [help] \n");

serial_print("setdate [setdate year month day] \n");
serial_print("settime [settime hour minute second] \n");


}

#include "help.h"

#include <stdint.h>
#include <string.h>


void help{
serial_print("command [version] [dettime] [getdate] [shutdown] [clear] [help] \n");

serial_print("setdate [setdate month day year] \n");
serial_print("settime [settime hour minute second] \n");


}

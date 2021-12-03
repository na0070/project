#include "serial2.h"
#include "new.h"
#include <core/serial.h>
#include <core/io.h>
#include <core/table.h>
#include <string.h>

u32int dev = COM1;
int level = 4;

struct dcb serial_dcb = {

}

u32int original_idt_entry;

int com_open(int baud_rate) {
    // error checking, baud rate is valid, port not already open
    cli();

}
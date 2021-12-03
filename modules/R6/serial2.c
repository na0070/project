#include "serial2.h"
#include "new.h"
#include "../mpx_supt.h"
#include <core/serial.h>
#include <core/io.h>
#include <core/tables.h>
#include <string.h>

u32int dev = COM1;
int level = 4;

struct dcb serial_dcb = {

}

u32int original_idt_entry;

int com_open(int baud_rate) {
    // error checking, baud rate is valid, port not already open
    cli();

    serial_dcb.open = 1;
    serial_dcb.events = 1;
    serial_dcb.status = IDLE;

    original_idt_entry = idt_get_gate(0x24);
    idt_set_gate(0x24,(u32int) top_handler, 0x08, 0x8e);

    long brd = 115200/(long) baud_rate;

    outb(dev+1,0); // disable ints

    outb(dev+3,0b10000000); // set line control register

    outb(dev,brd); //lsb

    outb(dev+1,brd>>8); //msb

    //lock divisor
    outb(dev+3,0b00000011);

    // enable FIFO, clear, 14byte threshold
    outb(dev+2,0b11000111);

    //enable PIC level
    outb(0x21, inb(0x21) & ~(1<<level));


    //enable read interrupt
    outb(dev+1,0b00000001);

    (void) inb(dev); // flush out the register

    sti(); // enable interrupts

    return 0;

    }
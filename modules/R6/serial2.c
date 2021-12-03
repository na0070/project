#include "serial2.h"
#include "new.h"
#include "../mpx_supt.h"
#include <core/serial.h>
#include <core/io.h>
#include <core/tables.h>
#include <string.h>

u32int dev = COM1;
int level = 4;

struct dcb serial_dcb;

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

void set_int(int bit, int on) {

    if (on) {
        outb(dev+1,inb(dev+1) | (1<<bit));

    }
    else {

        outb(dev+1,inb(dev+1) & ~(1<<bit));
    }
}

void input_h() {

    char i = inb(dev);
    outb(dev,i);
}



void top_handler() {

<<<<<<< HEAD
        // outb(dev,'b');
=======
    outb(dev,'b');
>>>>>>> 49182a0a6f89d0ae39d8a92407ca185593044bbc

    if (serial_dcb.open) {  // if open
        cli();

        int type = inb(dev+2);
        int bit1 = type>>1 & 1;
        int bit2 = type>>2 & 1;

<<<<<<< HEAD
            if (!bit1 && !bit2) {
                // modem
                inb(dev+6);
            }
            else if (bit1 && !bit2) {
                // 01 : output
                //call output handler
                //second_write();
                outb(COM1,'w');
                outb(COM1,':');
                outb(COM1,' ');
                // input_h();

            }

            else if (!bit1 && bit2) {
                // 10: input
                // call input handler
                // outb(COM1,'r');
                // outb(COM1,':');
                outb(COM1,' ');
                input_h();
            }
            else if (bit1 && bit2){
                // line
                inb(dev+5);
            }
            //klogv("int");
=======
        if (!bit1 && !bit2) {
            // modem
            inb(dev+6);
        }
        else if (bit1 && !bit2) {
            // 01 : output
            //call output handler
            second_write();
        }

        else if (!bit1 && bit2) {
            // 10: input
            // call input handler
            input_h();
        }
        else if (bit1 && bit2){
            // line
            inb(dev+5);
        }
        //klogv("int");
>>>>>>> 49182a0a6f89d0ae39d8a92407ca185593044bbc

        

<<<<<<< HEAD
            // char in = inb(dev);
            // outb(dev,in);
            // (void) in;
=======
        char in = inb(dev);
        outb(dev,in);
        // (void) in;
>>>>>>> 49182a0a6f89d0ae39d8a92407ca185593044bbc



        sti();

    }

        set_int(1,0);

    outb(0x20,0x20);
}
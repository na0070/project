// functions for R6
#include "func6.h"

// DCB global struct

DCB* device;


// com open function

int com_open(int* eflag_p, int baud_rate) {

    //1. error checking
    if (eflag_p == NULL)
        return INVALID_EFLAG;

    if (baud_rate != 110 && baud_rate != 150 && baud_rate != 300 && baud_rate != 600 && baud_rate != 1200 && baud_rate != 2400 && baud_rate != 4800 && baud_rate != 9600 && baud_rate != 19200)
        return INVALID_BRD;

    if (device.port == OPEN)
        return PORT_ALREADY_OPEN;

    //2. initialize the DCB
    device.port = OPEN;                                 // indicate device is open

    device.event_flag = 1;                              // device is not being used

    device.curr_op = IDLE;                              // device status is idle

    memset(device.buffer,'\0',sizeof(device.buffer));   // initialize and free the DCB's ring buffer


    //3. getting address of current interrupt handler (not needed)

    //4. compute baud rate divisor (BRD) using the baud rate: BRD = 115200 / baud_rate
    int BRD = 115200/baud_rate;

    //5. store value 0x80 in line control register (base+3) to access BRD registers
    outb(COM1+3,0x80);

    //6. store high order and low order BRD in correct registers
    outb(COM1,BRD);    // lower byte stores BRD
    outb(COM1+1,0x00); // higher byte stores 0

    //7. store 0x03 in line control register to access data registers again (base and base+1)
    outb(COM1+3,0x03);

    //8. enable PIC level (level 4)
    PIC(0x10);  // set bit 4 of PIC


    //9. store 0x08 in modem control register to allow overall serial port interrupts
    outb(COM1+4,0x08);

    //10. store 0x01 in interrupt enable register to allow input ready interrupts only
    outb(COM1+1,0x01);


    //end of function
    return 0;       
}






// com_write function

int com_write(char *buf_p, int *count_p)
{

    // get first character from requestor’s buffer
    // store to output buffer
    // enable the ‘write’ interrupt
    // ‘write’ interrupt handler performs rest

    // note: sys_req WRITE also uses buffer pointer and count pointer

    // int count = *count_p;       // store value of count into a local variable for future use
    // char* ptr = buf_p;          // create a loop pointer sstarting at buffer's location (may not even use it)

    /* from document
        1-(done) ensure that input parameters  are valid 
        2-(done) ensure that port is open and idle
        3-(done) install buf_p and count_p to DCB, and set current status to writing
        4-(done) clear caller's event flag
        5-(done) get first character from requestor's buffer and store it in output register
        6-(done) enable write interrupts by setting bit 1 of interrupt enable register (set register to OR of previous then AND 0x02)
    */

    // step 1: ensure the parameters are valid
    if (buf_p == NULL) // checking if buffer pointer is valid
        return INVALID_BUFF_ADDRESS;

    if (count_p == NULL) // checking if count pointer is valid
        return INVALID_COUNT;


    // step 2: check that the port is open and idle    
    if (device.port == NOT_OPEN)                  // check the device port status (0 = not open, 1 = open)
        return PORT_NOT_OPEN;

    if (device.curr_op != IDLE) // check if the device is currently busy (not idle)
        return DEVICE_BUSY;

    // step 3: install pointers to DCB and set status to writing
    device.buffer = buf_p;  // set the device's buffer pointer
    device.count = count_p; // set the device's count pointer
    device.curr_op = WRITE; // det the device's current operation

    // step 4: clear the caller's event flag
    device.event_flag = 0; // clear the device's event flag

    // change the count (# of characters to print) before enabling the interrupt
    device.count = device.count - 1;

    // step 5: get first character from requestor's buffer and store in output register
    outb(COM1, *buf_p); // or buf_p[0] if it not happy

    // step 6: enable write interrupts
    outb(COM1 + 1, (inb(COM1 + 1) | 0x02)); // inb(COM1+1) takes the previous value, ORing it with 0x02 to set bit #1

    // com_write is called once, and the handler does the worok of printing the remaining characters in the string
    // based on device.count's value (which is why we decrement before enabling the interrupt)
    
    return 0;                   // success return code
}



// function to change the PIC level
void PIC(int value) {

    cli();  // disable all interrupts

    int mask = inb(0x21);
    mask = mask & !value;
    outb(0x21,mask);
    outb(0x20,0x20);

    sti();  // enable all interrupts



}
// functions for R6
#include "func6.h"

// DCB global struct

DCB* device;




// com_write function

int com_write (char* buf_p, int* count_p) {

// get first character from requestor’s buffer
// store to output buffer
// enable the ‘write’ interrupt
// ‘write’ interrupt handler performs rest

// note: sys_req WRITE also uses buffer pointer and count pointer


    int count = *count_p;       // store value of count into a local variable for future use
    char* ptr = buf_p;          // create a loop pointer sstarting at buffer's location (may not even use it)

    /* from document
        1-(done) ensure that input parameters  are valid 
        2-(done) ensure that port is open and idle
        3-(done) install buf_p and count_p to DCB, and set current status to writing
        4-(done) clear caller's event flag
        5-(done) get first character from requestor's buffer and store it in output register
        6-(done) enable write interrupts by setting bit 1 of interrupt enable register (set register to OR of previous then AND 0x02)
    */

    // step 1: ensure the parameters are valid
    if (buf_p == NULL)                     // checking if buffer pointer is valid
        return INVALID_BUFF_ADDRESS;

    if (count_p == NULL)                   // checking if count pointer is valid
        return INVALID_COUNT;


    // step 2: check that the port is open and idle    
    if (device.port == 0)                  // check the device port status (0 = not open, 1 = open)
        return PORT_NOT_OPEN;

    if (device.curr_op != IDLE)            // check if the device is currently busy (not idle)
        return DEVICE_BUSY;
    

    // step 3: install pointers to DCB and set status to writing
    device.buffer = buf_p;                  // set the device's buffer pointer
    device.count = count_p;                 // set the device's count pointer
    device.curr_op = WRITE;                 // det the device's current operation

    // step 4: clear the caller's event flag
    device.event_flag = 0;                  // clear the device's event flag

    // step 5: get first character from requestor's buffer and store in output register
    for (int i = 1; i <= count; i++) {
        outb(COM1,*buf_p);

    // step 6: enable write interrupts
        outb(COM1+1, (inb(COM1+1) | 0x02) ) ;   // inb(COM1+1) takes the previous value, ORing it with 0x02 to set bit #1


        buf_p++;            // point to next character in buffer

    }









    // modify count's value after transfer


    return 0;                   // success return code
}
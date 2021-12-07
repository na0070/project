// functions for R6
#include "func6.h"
#include "../mpx_supt.h"

// DCB global struct

DCB device = {.internal_loc = 0};


int level = 4;
u32int original_idt_entry;

IOqueue IOlist = {.head = NULL, .tail = NULL};

// int IOcount = 0;        // counter indicating number of requests made (for IO scheduler)


int com_open(int baud_rate) {
    // error checking; baud rate is valid, port not already open

    if (baud_rate != 110  && baud_rate != 150  && baud_rate != 300 && 
        baud_rate != 600  && baud_rate != 1200 && baud_rate != 2400 && 
        baud_rate != 4800 && baud_rate != 9600 && baud_rate != 19200)
        return INVALID_BRD;

    if (device.port == OPEN)
        return PORT_ALREADY_OPEN;


    cli();

    device.port = 1;            // open the port
    device.event_flag = 1;      // set the event flag (not doing anything)
    device.current_op = IDLE;   // set the device state to idle

    original_idt_entry = idt_get_gate(0x24);            // set the interrupt vector to the top_handler (whenever reading/writing)
    idt_set_gate(0x24,(u32int) top_handler, 0x08, 0x8e);

    long brd = 115200/(long) baud_rate;                 // compute baud rate divisor

    outb(COM1+1,0); // disable ints

    outb(COM1+3,0b10000000); // set line control register

    outb(COM1,brd); //lsb

    outb(COM1+1,brd>>8); //msb

    //lock divisor
    outb(COM1+3,0b00000011);

    // enable FIFO, clear, 14byte threshold
    outb(COM1+2,0b11000111);

    //enable PIC level
    outb(0x21, inb(0x21) & ~(1<<level));


    //enable read interrupt
    outb(COM1+1,0b00000001);

    (void) inb(COM1); // flush out the register

    sti(); // enable interrupts

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
        return INVALID_COUNT_P;


    // step 2: check that the port is open and idle    
    if (device.port == NOT_OPEN)                  // check the device port status (0 = not open, 1 = open)
        return PORT_NOT_OPEN;

    if (device.current_op != IDLE) // check if the device is currently busy (not idle)
        return DEVICE_BUSY;

    // step 3: install pointers to DCB and set status to writing
    device.user_buffer = buf_p;  // set the device's buffer pointer
    device.count = count_p; // set the device's count pointer
    device.current_op = WRITE; // det the device's current operation
    device.current_loc = 0;

    // step 4: clear the caller's event flag
    device.event_flag = 0; // clear the device's event flag

    device.buffersize = *device.count;      // set device's buffersize to given count value

    // change the count (# of characters to print) before enabling the interrupt
    // *device.count = *device.count - 1;

    // step 5: get first character from requestor's buffer and store in output register
    outb(COM1, device.user_buffer[device.current_loc]); // or buf_p[0] if it not happy

    device.current_loc++; // start at beginning of array for second_write to use


    // step 6: enable write interrupts
    outb(COM1+1, (inb(COM1+1) | 0x02)); // inb(COM1+1) takes the previous value, ORing it with 0x02 to set bit #1

    // com_write is called once, and the handler does the work of printing the remaining characters in the string
    // based on device.count's value (which is why we decrement before enabling the interrupt)
    
    return 0;                   // success return code
}


// IO scheduler
int IOscheduler() {
    // klogv("IO scheduler: IN");

    IOCB* request = IOlist.head;
    // klogv("IO scheduler: after request pointer"); 

    if (device.port != OPEN || device.current_op != IDLE || request == NULL) {
        // klogv("IO scheduler: -1");
        return -1;
    }

    // klogv("IO scheduler: after error check");

    if (device.event_flag == 1) {   // if event flag == 1, an IO was completed, go to next request

        // klogv("IO Scheduler: inside event_flag == 1");

        // if (IOcount != 0) {                            // if not the very first request (edge case) otherwise, call com_read/write immediately 

        IOCB* temp = request;
            
            removePCB(request -> process);             // remove process from current list
            request -> process -> state = READY;       // unblock the prcoess first
            insertPCB(request -> process);             // reinsert the process back to the correct list
            request = request -> next;
            sys_free_mem(temp);
            IOlist.head = request;
            // klogv("IO count != 0");
        // }


        // klogv("IO scheduler: after initialization");
        if (request != NULL) {
            if (request -> op == IDLE || request -> op == EXIT)
                return -1;

            if (request -> buffer == NULL)
                return -1;

            // klogv("after error check");
            if (request -> op == READ) {
                // klogv("iO scheduler: READ");
                com_read(request -> buffer, request -> count);
            }
            if (request -> op == WRITE) {
                // klogv("iO scheduler: WRITE");
                com_write(request -> buffer, request -> count);
            }

            // if (IOcount == 0) {
            //     // klogv("IO count == 0");
            //     IOcount++; 
            // }
        }
        
    } 
    // klogv("IO Scheduler: outside event == 1");
    // if event flag is still 0, then device still being used
    // do nothing instead
    // klogv("IO scheduler: END");
    return 0;
}

// load an IOCB to the queue
void loadIOCB(pcb* proc, int code, char* buff, int* count) {

    IOqueue* queue = &IOlist;
    // klogv("load IOCB: BEFORE sys_alloc_mem");
    IOCB* request = (IOCB*)sys_alloc_mem(sizeof(IOCB));     // initialize the IOCB with given values
    // klogv("load IOCB: AFTER sys_alloc_mem");
    request -> device = &device;
    request -> process = proc;
    request -> op = code;
    request -> buffer = buff;
    request -> count = count;

    if (queue->head == NULL) {           // if queue is empty
        // klogv("loadIOCB: queue is empty");
        queue->head = request;
        queue->tail = request;

        if (code == READ) {
            // klogv("LoadIOCB: come_read");
            com_read(buff,count);
        }
        else {
             // klogv("LoadIOCB: come_write");
            com_write(buff,count);
        }
    }
    else {                              // if queue is not empty
        // klogv("loadIOCB: queue is NOT empty");
        queue->tail->next = request;
        queue->tail = request;
    }

    request->next = NULL;

    // IOcount++;      // increment number of IO requests made

    // klogv("load IOCB: END");
}

// function to change the PIC level
void PIC(int value) {

    cli();  // disable all interrupts

    int mask = inb(0x21);
    mask = mask & ~value;
    outb(0x21,mask);

    sti();  // enable all interrupts

    outb(0x20,0x20);    // EOI signal

}

int com_read (char* buf_p, int* count_p) {
	
	if (buf_p == NULL){
		
		return -302;
	}
	
		if (count_p == NULL){
		
		return -303;
	}
	
	if (device.port != OPEN ){
		
		return -301;
	}
	
	if (device.current_op != IDLE) {
		
		return -304;
	}

    // klogv("com_read: after error check");

	// memset(device.user_buffer, '\0', *count_p); // initialize the user buffer to be filled 
	device.user_buffer = buf_p;
    device.count = count_p;

    set_int(0,0);       // disable read interrupt
    cli();              // diaasble interrupts for now
    

	device.current_op = READ; 
	device.event_flag = 0;

    device.transferred = 0;
    // transfer characters from ring buffer to user buffer
    int i;
    for (i = 0; i < device.internal_loc; i++) {
        device.user_buffer[i] = device.internal_buff[i];
        device.transferred++;
        device.internal_buff[i] = '\0'; // remove character from ring buffer after copying

        if (device.transferred == *count_p || device.user_buffer[i] == '\r') // stop reading from ring buffer if reached the target number of characters or CR entered
            break;
    } 

     // klogv("com_read: after ring buffer transfer");

    if (device.transferred == *count_p) {       // if reached the reading target
        // klogv("com_read: transferred == *count_p");
        device.current_op = IDLE;
        device.event_flag = 1;
        device.user_buffer = NULL;              // user buffer shouldn't point to requestor buffer anymore
        device.count = NULL;                    // count pointer shouldn't point to count_p anymore
        device.internal_loc = 0;                // reset ring buffer index
    }

    sti();              // enable interrupts
    set_int(0,1);       // enable read interrupt
	 
     return 0;
}


  // com close 
int com_close (void){
// from the document 

//     1. Ensure that the port is currently open. 
if (device.port != OPEN){
        return PORT_NOT_OPEN;}//if 
 else{    
   // 2. Clear the open indicator in the DCB. 
    device.port = NOT_OPEN;  // indicate device is close

    device.event_flag = 0;    // device is being used

    device.current_op = IDLE;    // device status is idle
    //3. Disable the appropriate level in the PIC mask register.
  PIC(0x01); 
   // 4. Disable all interrupts in the ACC by loading zero values to the Modem Status register and the Interrupt Enable register.
   cli(); 
   // 5. Restore the original saved interrupt vector. (not needed)
   
return 0; } // else 
}// com close 

void second_write() {

    //  1. If the current status is not writing, ignore the interrupt and return. 
 
    if (device.current_op != WRITE)
	    return;// if 

    else {
    //2. Otherwise, if the count has not been exhausted, get the next character from the requestor's output buffer and store it in the output register. Return without signaling completion. 

        if (device.current_loc < device.buffersize ){
            char data = device.user_buffer[device.current_loc];
            outb(COM1,data);
            device.current_loc++;
            // device.current_loc = device.current_loc+1;
        }// if 
        else{
            //  3. Otherwise, all characters have been transferred. Reset the status to idle. Set the event flag and return the count value. Disable write interrupts by clearing bit 1 in the interrupt enable register.
            device.current_op = IDLE; 
            device.event_flag = 1; 
            inb(COM1);

            set_int(1,0); // turn off interrupt
            
        }//else 2     

    }// else     
}// second write 

// first level interrupt handler
void top_handler() {


    if (device.port) {  // if open
        cli();     // disable interrupts

        int type = inb(COM1+2); // read the type of interrupt
        int bit1 = type>>1 & 1; // separate the 2 important bits (bit 1 and 2)
        int bit2 = type>>2 & 1;

            // check the combination of bits 1 and 2 to determine the cause of interrupt
            if (!bit1 && !bit2) {
                // modem interrupt
                inb(COM1+6);
            }
            else if (bit1 && !bit2) {
                // 01 : output
                //call output handler
                
                // outb(COM1,'w');
                // outb(COM1,':');
                // outb(COM1,' ');
                second_write();
                // input_h();

            }

            else if (!bit1 && bit2) {
                // 10: input
                // call input handler
                // outb(COM1,'r');
                // outb(COM1,':');
                // outb(COM1,' ');
                input_h();
            }
            else if (bit1 && bit2){
                // line interrupt
                inb(COM1+5);
            }
            //klogv("int");

        

            // char in = inb(COM1);
            // outb(COM1,in);
            // (void) in;



        sti();  // enable all interrupts

    }

        // set_int(1,0);   // for now, turn off write interrupt (removed later)

    outb(0x20,0x20);        // EOI
}

// set interrupt on or off
void set_int(int bit, int on) {

    if (on) {
        outb(COM1+1,inb(COM1+1) | (1<<bit));

    }
    else {

        outb(COM1+1,inb(COM1+1) & ~(1<<bit));
    }
}


// "input handler" == second_read
void input_h() { 

    char input = inb(COM1);
    outb(COM1,input);                   // may want to make special character prints here

    if (device.current_op != READ) {    // device is not reading (sys_req READ not called yet, but a key was pressed)
        // store in ring buffer for later use (internal_buff)
        if (device.internal_loc < strlen(device.internal_buff)) {

            device.internal_buff[device.internal_loc] = input;
            device.internal_loc++;
        }
        return;
    }

    else {                              // device is reading (sys_req READ was called)
        // store in requestor buffer instead (user_buffer)
        device.user_buffer[device.internal_loc] = input;
        device.internal_loc++;      // incremenet first since character at internal_loc already written in buffer
        
        

        // if count not reached and not CR, then reading not complete; return
        if (device.internal_loc < *device.count-1 && input != '\r')
            return;

        else {          // otherwise, transfer is complete, signal completion
            // klogv("test");
            device.current_op = IDLE;
            device.event_flag = 1;      // signal end of operation
            device.user_buffer[device.internal_loc-1] = '\0';
            *device.count = device.internal_loc + 1; // return modified count value
            device.internal_loc = 0; // reset current location
            // device.user_buffer = NULL;
            // device.count = NULL;

            outb(COM1,'\n');
            //inb(COM1);


            // outb(COM1,'\n');
            // klogv("");
            // outb(COM1,'\n');
            // com_write(device.user_buffer,device.count);


        }
    }


    
    
}
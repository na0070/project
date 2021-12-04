// functions for R6
#include "func6.h"
#include "../mpx_supt.h"

// DCB global struct

DCB device;

int level = 4;
u32int original_idt_entry;

IOqueue IOlist = {.head = NULL, .tail = NULL};

// com open function
// OLD com_open
// int com_open(int* eflag_p, int baud_rate) {

//     //1. error checking
//     if (eflag_p == NULL)
//         return INVALID_EFLAG;

//     if (baud_rate != 110  && baud_rate != 150  && baud_rate != 300 && 
//         baud_rate != 600  && baud_rate != 1200 && baud_rate != 2400 && 
//         baud_rate != 4800 && baud_rate != 9600 && baud_rate != 19200)
//         return INVALID_BRD;

//     if (device.port == OPEN)
//         return PORT_ALREADY_OPEN;

//     //2. initialize the DCB
//     device.port = OPEN;                                 // indicate device is open

//     device.event_flag = eflag_p;                              // device is not being used

//     device.current_op = IDLE;                              // device status is idle

//     memset(device.internal_buff,'\0',sizeof(device.internal_buff));   // initialize and free the DCB's ring buffer


//     //3. getting address of current interrupt handler (not needed)

//     //4. compute baud rate divisor (BRD) using the baud rate: BRD = 115200 / baud_rate
//     int BRD = 115200/baud_rate;

//     //5. store value 0x80 in line control register (base+3) to access BRD registers
//     outb(COM1+3,0x80);

//     //6. store high order and low order BRD in correct registers
//     outb(COM1,BRD);    // lower byte stores BRD
//     outb(COM1+1,0x00); // higher byte stores 0

//     //7. store 0x03 in line control register to access data registers again (base and base+1)
//     outb(COM1+3,0x03);

//     //8. enable PIC level (level 4)
//     PIC(0x10);  // set bit 4 of PIC


//     //9. store 0x08 in modem control register to allow overall serial port interrupts
//     outb(COM1+4,0x08);

//     //10. store 0x01 in interrupt enable register to allow input ready interrupts only
//     outb(COM1+1,0x01);


//     //end of function
//     return 0;       
// }

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

    // step 4: clear the caller's event flag
    device.event_flag = 0; // clear the device's event flag

    device.buffersize = *device.count;      // set device's buffersize to given count value

    // change the count (# of characters to print) before enabling the interrupt
    *device.count = *device.count - 1;

    // step 5: get first character from requestor's buffer and store in output register
    outb(COM1, *device.user_buffer); // or buf_p[0] if it not happy

    device.current_loc = 1; // start at beginning of array for second_write to use


    // step 6: enable write interrupts
    outb(COM1+1, (inb(COM1+1) | 0x02)); // inb(COM1+1) takes the previous value, ORing it with 0x02 to set bit #1

    // com_write is called once, and the handler does the work of printing the remaining characters in the string
    // based on device.count's value (which is why we decrement before enabling the interrupt)
    
    return 0;                   // success return code
}


// IO scheduler
int IOscheduler() {

    IOCB* request = IOlist.head;

    if (device.port != OPEN || device.current_op != IDLE || request == NULL)
        return -1;

    if (device.event_flag == 1) {   // if event flag == 1, an IO was completed, go to next request

        request -> process -> state = READY;       // unblock the prcoess first
        removePCB(request -> process);             // remove process from current list
        insertPCB(request -> process);             // reinsert the process back to the correct list
        request = request -> next;
        IOlist.head = request;

        if (request -> op == IDLE || request -> op == EXIT)
            return -1;

        if (request -> buffer == NULL)
            return -1;

        if (request -> op == READ)
            // com_read(request -> buffer, request -> count);

        if (request -> op == WRITE)
            com_write(request -> buffer, request -> count);

    } 
    // if event flag is still 0, then device still being used
    // do nothing instead
    
    return 0;
}

// load an IOCB to the queue
void loadIOCB(pcb* proc, int code, char* buff, int* count) {

    IOqueue* queue = &IOlist;

    IOCB* request = (IOCB*)sys_alloc_mem(sizeof(IOCB));     // initialize the IOCB with given values

    request -> device = &device;
    request -> process = proc;
    request -> op = code;
    request -> buffer = buff;
    request -> count = count;

    if (queue->head == NULL) {           // if queue is empty
        queue->head = request;
        queue->tail = request;
    }
    else {                              // if queue is not empty
        queue->tail->next = request;
        queue->tail = request;
    }

    request->next = NULL;
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
	
	char* buffer; 
	
	device.current_op = READ; 
	
	device.event_flag = 0;
	
	// strcpy(buffer, device.internal_buff); 

    // copy from ring to requester (buf_p)
	
	if (count_p != 0){
		
		return 0; 
	}
	
	device.current_op = IDLE; 
	
	device.event_flag = 1; 
	
	return *count_p ;  
}



/*

int second_read (char*	buf_p, int *count_p) {

	// int Second_read (int *count_p) {
	char input = inb(COM1); 
	
		if (device.current_op != READ){
			// use the internal buffer
			device.buffer [count_p] = input; 
			
				return -1;
		}
        // else, use the user buffer
		
		buf_p [count_p] = input; 
		
		if (count_p	!= 0 && input != '\n'){
			
			return -1;
		}
		
		device.current_op = IDLE; 
		
			device.event_flag = 1; 
			
			return *count_p; 
}



*/



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

            set_int(1,0); // turn off interrupt
            
        }//else 2     

    }// else     
}// second write 


// OLD first_handler

// first level interrupt handler
// void first_handler() {

//     //1. if port is not open, clear interrupt and return
//     if (device.port == NOT_OPEN) {

//         //clear interrupt
//         outb(0x20,0x20); // EOI?
//         return;
//     }


//     //2. read interrupt ID register to determine cause of interrupt

//     int ID = inb(COM1+2);

//     // bit 0 must be a 0 if interrupt caused by port before proceeding
//     if ((ID & 0b0001) == 0) {
//         // bits 2 and 1 being 01 means output interrupt
//         if ((ID & 0b0110) == 0b0010) {
//             // output interrupt
//             second_write();
//         }
//         // bits 2 and 1 being 10 means input interrupt
//         if ((ID & 0b0110) == 0b0100) {
//             // input interrupt
//             second_read();
//         }

//     //4. call EOI for PIC
//         outb(0x20,0x20);

//     }
// }

// NEW first level interrupt handler
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


// "input handler"
void input_h() { 

    char i = inb(COM1);
    outb(COM1,i);
}
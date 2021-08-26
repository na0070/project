/*
  ----- serial.c -----

  Description..: Contains methods and variables used for
    serial input and output.
*/

#include <stdint.h>
#include <string.h>

#include <core/io.h>
#include <core/serial.h>

//temporary
#include <modules/mpx_supt.h>

#define NO_ERROR 0

// Active devices used for serial I/O
int serial_port_out = 0;
int serial_port_in = 0;

/*
  Procedure..: init_serial
  Description..: Initializes devices for user interaction, logging, ...
*/
int init_serial(int device)
{
  outb(device + 1, 0x00); //disable interrupts
  outb(device + 3, 0x80); //set line control register
  outb(device + 0, 115200/9600); //set bsd least sig bit
  outb(device + 1, 0x00); //brd most significant bit
  outb(device + 3, 0x03); //lock divisor; 8bits, no parity, one stop
  outb(device + 2, 0xC7); //enable fifo, clear, 14byte threshold
  outb(device + 4, 0x0B); //enable interrupts, rts/dsr set
  (void)inb(device);      //read bit to reset port
  return NO_ERROR;
}

/*
  Procedure..: serial_println
  Description..: Writes a message to the active serial output device.
    Appends a newline character.
*/
int serial_println(const char *msg)
{
  int i;
  for(i=0; *(i+msg)!='\0'; i++){
    outb(serial_port_out,*(i+msg));
  }
  outb(serial_port_out,'\r');
  outb(serial_port_out,'\n');  
  return NO_ERROR;
}

/*
  Procedure..: serial_print
  Description..: Writes a message to the active serial output device.
*/
int serial_print(const char *msg)
{
  int i;
  for(i=0; *(i+msg)!='\0'; i++){
    outb(serial_port_out,*(i+msg));
  }
  if (*msg == '\r') outb(serial_port_out,'\n');
  return NO_ERROR;
}

/*int (*student_read)(char *buffer, int *count);
  Procedure..: set_serial_out
  Description..: Sets serial_port_out to the given device address.
    All serial output, such as that from serial_println, will be
    directed to this device.
*/
int set_serial_out(int device)
{
  serial_port_out = device;
  return NO_ERROR;
}

/*
  Procedure..: set_serial_in
  Description..: Sets serial_port_in to the given device address.
    All serial input, such as console input via a virtual machine,
    QEMU/Bochs/etc, will be directed to this device.
*/
int set_serial_in(int device)
{
  serial_port_in = device;
  return NO_ERROR;
}

int *polling(char *buffer, int *count){
// insert your code to gather keyboard input via the technique of polling.
// You must validat each key and handle special keys such as delete, back space, and
// arrow keys

		int i = 0; //index
		int counter = *count; // size of buffer (max number of characters allowewd)
		
		serial_print("Start typing:\n"); // prompt message to begin typing
		
		while (counter > 0) { // while buffer is not full
			
			if (inb(COM1+5)&1) {	//if data is available in COM1
			
			buffer[i] = inb(COM1);	// store the data (character) in the next empty space in buffer
			
			
				if (buffer[i] == '\r') {	// if enter key is pressed
					buffer[i] = '\n';		// set a new line where we stopped at in buffer
					buffer[i+1] = '\0';		// add the terminating character to indicate end of string
											
					break; 					// good bye
				}
				else {	// other keys
					
					i = i+1;	// go to next empty location in buffer		
					counter = counter-1;	// reduce max number of empty spaces by one
					
				}
			
			}
		}
		
	/*
		issues/to do:
				- still not sure how to print one character at a time
				- not sure how to "un-print" a character after printing it on terminal
				- how to show in terminal that the cursor is moving left/right
				- have to add other special functionality keys
				- know how to create/edit makefiles for new .c files
				- how to properly exit the program from terminal after "shutdown" instead of closing/reopening
	*/
	
	
// remove the following line after implementing your module, this is present
// just to allow the program to compile before R1 is complete
strlen(buffer);

return count;
}


// temporary

void comm() {
	char buffer[80];
	int size = 80;
	
	sys_req(READ,DEFAULT_DEVICE,buffer,&size);
	
	sys_req(WRITE,DEFAULT_DEVICE,buffer,&size);
	return;

}


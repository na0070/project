/**
  ----- serial.c -----

  Description..: Contains methods and variables used for
    serial input and output.
*/

#include <stdint.h>
#include <string.h>

#include <core/io.h>
#include <core/serial.h>



#define NO_ERROR 0

// Active devices used for serial I/O
int serial_port_out = 0;
int serial_port_in = 0;

/**
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

/**
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

/**
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
// You must validate each key and handle special keys such as delete, back space, and
// arrow keys

		int i = 0; //index
		int counter = *count; // size of buffer (max number of characters allowewd)
		
		serial_print("Enter command below: \n"); // prompt message to begin typing
		
		while (counter > 0) { // while buffer is not full
			
			if (inb(COM1+5)&1) {	//if data is available in COM1
			
				char letter = inb(COM1);	// store the data into the variable
					
			
					if (letter == '\r') {	// if enter key is pressed
						buffer[*count-1] = '\0';		// end the buffer
						serial_print("\n");
												
						break; 					// good bye
					}
					
					
					else if (letter == '\x7F' || letter == '\b') { // if DEL or backspace is entered
						
						
						if (i > 0)  i = i-1;	// point one character to the left in buffer
						if (counter < *count) counter = counter + 1;	// indicate therer is one more free space
						
						int j = i;	//temporarily save current index location
						
						serial_print("\b");			// bring cursor one character back (to the left)
						serial_print("\x1B[s");		// save the current cursor location
						serial_print("\x1B[2K");	// visually delete the entire line in terminal
						serial_print("\x1B[0E");	// move cursor to beginning of bottom line
						serial_print("\x1B[A");		// go up a line (to begin at original line)
						
						
						while (i < *count-1) {		//loop through the buffer from where we want to delete
						
							buffer[i] = buffer[i+1];	// move every character after deletion point one space left
							i = i+1;
						}
						
						buffer[i] = '\0';	// for safety, set last character to NULL
						i = j;		// return index to original location
						
						inb(COM1);	// flush out COM1
						
						serial_print(buffer);		// print the newly editted buffer
						serial_print("\x1B[u");		// bring cursor back to saved position
						
					}
					
					else if (letter == '\x1B') { // if arrow keys are pressed
						
						letter = inb(COM1);		//read whats after the ESC code
						
						if (letter == '[') {	
							letter = inb(COM1);
							
							if (letter == 'D') {	//left arrow is pressed "\x1B[D"
								
								if (i > 0)  i = i-1; //point index one character to the left in the buffer
								serial_print("\x1B[D");	//move the curser itself on the terminal left
								
								
							}
							else if (letter == 'C') {	//right arrow is pressed "\x1B[C"
								
								if (buffer[i] != '\0' && i < *count-1)  { //don't move right if cursor reached the very edge
									
									i = i+1; //point one character to the right in the buffer
									serial_print("\x1B[C");	//move the curser itself on the terminal right
								}
							}
							else {} //dont do anything if its up/down arrows (for now)
							
						}
						
						inb(COM1);	//flush out any COM1
						
					}
					
					else {	// other keys (printable), will need to push everything to the right if writing from middle
					
					int j = i;	//temporarily save current index location
					
					
					i = *count-1;	// start from the end of the buffer
					
					while (i > j) {	// loop from end of buffer down until printing location
						
						buffer[i] = buffer[i-1];	// move every character after insertion point one space right
						i = i-1;
					}
					
					buffer[j] = letter;		// store the new letter at printing location
					
					
						serial_print("\x1B[C");		// move cursor one character to the right
						serial_print("\x1B[s");		// save the current cursor location
						serial_print("\x1B[2K");	// visually delete the line
						serial_print("\x1B[0E");	// move cursor to beginning of bottom line
						serial_print("\x1B[A");		// go up a line (to begin at original line)
					
						
						serial_print(buffer);		// visually print the newly editted buffer
						serial_print("\x1B[u");		// bring cursor back to saved location
						
						if (i < *count-1) i = j+1;	// go to next location in buffer		
						counter = counter-1;		// reduce max number of empty spaces by one
						
					}
					
			
			}
		}
		

return count;
}




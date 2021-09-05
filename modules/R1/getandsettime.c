
#include "../mpx_supt.h"

#include "getandsettime.h"

#include <core/io.h>

#include <core/serial.h>

#define Sec 0x00	//!< The RTC register corresponding to seconds

#define Min 0x02	//!< The RTC register corresponding to minutes

#define Hr 0x04		//!< The RTC register corresponding to hours

/*!
\brief Grabs the hours, minutes, and seconds values of the time it is invoked and fills a buffer with that information.

Since the information in the RTC registers are stored in BCD form, they will need to be converted back to regular binary.
Two copies of the hour value are stored in two int's, one to represent the "tens" location and the other for the "ones" location.
With binary operations, the tens int will correctly represent the binary code for its number, and the same for the ones int.
The process is repeated again for minutes and seconds after the two digits are stored in the buffer and the pointer is moved
forward by two locations.

@param *ptr A pointer to the buffer to be filled with time information
*/
void get_Time (int * pointer) {
	
	
	int tens, ones;
	
			outb(0x70, Hr) ; // Hours  
	
				tens = inb(0x71); // first segment of the Hours 
		
					ones = inb(0x71); // second segment of the Hours
			
			tens = (tens >> 4) + '0' ;  // the tens position of Hours 
			
		ones = (ones & 0x0f ) + '0' ; // the ones position of Hours 
		
		*pointer = tens;
		
			pointer = pointer +1;
		
				*pointer = ones; 
				
							pointer = pointer +1;
				
	/* --------------------------------------------------------------------------------------------------------------------------------------------- */			
		
			outb(0x70, Min) ; // Minutes  
	
				tens = inb(0x71); // first segment of the Minutes 
		
					ones = inb(0x71); // second segment of the Minutes
			
			tens = (tens >> 4) + '0' ;  // the tens position of Minutes 
			
		ones = (ones & 0x0f ) + '0' ; // the ones position of Minutes 
		
		*pointer = tens;
		
			pointer = pointer +1;
		
				*pointer = ones; 
				
			pointer = pointer +1;
			
	/* --------------------------------------------------------------------------------------------------------------------------------------------- */			

	outb(0x70, Sec) ; // seconds  
	
				tens = inb(0x71); // first segment of the second 
		
					ones = inb(0x71); // second segment of the second
			
			tens = (tens >> 4) + '0' ;  // the tens position of seconds 
			
		ones = (ones & 0x0f ) + '0' ; // the ones position of seconds 
		
		*pointer = tens;
		
			pointer = pointer +1;
		
				*pointer = ones; 
						
}


/*!
\brief Changes the current time of the machine to the provided values.

To change the time, similarly to date, the interrupts are first disabled, then the given int values are converted to their respective BCD form.
Once this is done, each int is then written to its corresponding register. Finally, the interrupts are enabled again.

@param hour int value of the hour
@param minutes int value of the minutes
@param seconds int value of the seconds
*/
void set_Time (int hour, int minutes, int seconds ){
	
	int s_time; 
	
	cli();
		
		s_time = (( hour / 10) << 4 ) | (hour % 10);
			outb (0x70, Hr);
				outb(0x71, s_time);
				
		s_time = (( minutes / 10) << 4 ) | (minutes % 10);
			outb (0x70, Min);
				outb(0x71, s_time);
				
		s_time = (( seconds / 10) << 4 ) | (seconds % 10);
			outb (0x70, Sec);
				outb(0x71, s_time);
				
	sti();

}



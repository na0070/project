
#include "../mpx_supt.h"

#include "getandsettime.h"

#include <core/io.h>

#include <core/serial.h>

#define Sec 0x00 

#define Min 0x02

#define Hr 0x04
 
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

Void set_Time (int hour, int minutes, int seconds ){
	
	int s_time; 
	
	cli();
		
		s_time = (( hour / 10) << 4 ) | (hour % 10);
			outb (0x70, Hr);
				outb(0x71, s_time);
				
		s_time = (( Minutes / 10) << 4 ) | (Minutes % 10);
			outb (0x70, Min);
				outb(0x71, s_time);
				
		s_time = (( seconds / 10) << 4 ) | (seconds % 10);
			outb (0x70, Sec);
				outb(0x71, s_time);
				
	sti();

}



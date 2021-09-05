
#include "../mpx_supt.h"
#include "getsetdate.h"

#include <core/io.h>
#include <core/serial.h>

/*!
\brief Grabs the year, month, and day values of the time it is invoked and fills a buffer with that information.

Since the information in the RTC registers are stored in BCD form, they will need to be converted back to regular binary.
Two copies of the year value are stored in two int's, one to represent the "tens" location and the other for the "ones" location.
With binary operations, the tens int will correctly represent the binary code for its number, and the same for the ones int.
The process is repeated again for month and day after the two digits are stored in the buffer and the pointer is moved
forward by two locations.

@param *ptr A pointer to the buffer to be filled with date information
*/
void getdate(int * ptr){
	
	int i;
	int j;
	outb(0x70, 0x09);
	i=inb(0x71);
	j=inb(0x71);
	i=(i>>4)+'0';
	j=(j&0x0f)+'0';
	*ptr=i;
	*(ptr+1)=j;
	ptr=ptr+2;
	
	outb(0x70, 0x08);
	i=inb(0x71);
	j=inb(0x71);
	i=(i>>4)+'0';
	j=(j&0x0f)+'0';

	*ptr=i;
	*(ptr+1)=j;
	ptr=ptr+2;
	outb(0x70, 0x07);
	i=inb(0x71);
	j=inb(0x71);
	i=(i>>4)+'0';
	j=(j&0x0f)+'0';

	*ptr=i;
	*(ptr+1)=j;
}

/*!
\brief Changes the current date of the machine to the provided values.

To change the date, the interrupts are first disabled, then the given int values are converted to their respective BCD form.
Once this is done, each int is then written to its corresponding register. Finally, the interrupts are enabled again.

@param year int value of the year
@param month int value of the month
@param day int value of the day
*/
void setdate(int year,int month, int day){
	int bcd = ((year /10)<<4)| (year % 10);
	cli();
	outb(0x70, 0x09);
	outb(0x71, bcd);
	
	bcd = ((month /10)<<4)| (month % 10);
	outb(0x70, 0x08);
	outb(0x71, bcd);
	
	bcd = ((day /10)<<4)| (day % 10);
	outb(0x70, 0x07);
	outb(0x71, bcd);
	sti();
}
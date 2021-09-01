
#include "../mpx_supt.h"
#include "getsetdate.h"

#include <core/io.h>
#include <core/serial.h>


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


void setdate(int year,int month, int day){
	//char i,j;
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
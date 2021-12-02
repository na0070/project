#
# Makefile for MPX Modules

AS      = nasm
CC      = i386-elf-gcc
CFLAGS  = -Wall -Wextra -Werror -nostartfiles -nostdlib -nodefaultlibs -ffreestanding -g -c
LD      = i386-elf-ld
LDFLAGS = 
ASFLAGS = -f elf -g

# add new files as you create them.
# I'll start you off with just mpx_supt.o
OBJFILES =\
mpx_supt.o\
R1/comm.o\
R1/getsetdate.o\
R1/getandsettime.o\
R1/help.o\
R1/version.o\
R2/func.o\
procsr3.o\
R4/Alarm.o\
R5/func5.o\
R6/func6.o\


.c.s:
	$(CC) $(CFLAGS) -S -o $@ $<
.s.o:
	$(AS) $(ASFLAGS) -o $@ $<
.c.o:
	$(CC) $(CFLAGS) -I../include -o $@ $<

all: modules.o

modules.o: $(OBJFILES)
	$(LD) -r -o modules.o $(OBJFILES)

clean:
	rm modules.o $(OBJFILES)

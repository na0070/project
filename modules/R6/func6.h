// header for func6.c


#include <core/serial.h>
#include <core/io.h>
#include <string.h>
#include <core/tables.h>
#include <system.h>

#include "../R2/func.h"

// error codes definitions

#define INVALID_EFLAG            -101
#define INVALID_BRD              -102
#define PORT_ALREADY_OPEN        -103

#define PORT_NOT_OPEN            -401
#define INVALID_BUFF_ADDRESS     -402
#define INVALID_COUNT_P          -403
#define DEVICE_BUSY              -404

#define NOT_OPEN    0
#define OPEN        1


// DCB struct
typedef struct dcb {
    int port;
 int allocation_st; // the current status of the device 0 for avalible 1 for in use 
  int current_op; // indicate the current opearation 
  int event_flag; //  indicate the status of the current operation and knows when the current operation is done so the next can start 
  char* user_buffer; // pointer to use buffer (call to sys_req)
  int* count;
  char internal_buff[20]; // it is array store charecter recieved from device 
  int internal_loc; // current lcoation of index for ring buffer
  int current_loc; // current location of the next charecter to transfer 
  int buffersize; // total numbe of charecter to read and write  
  int transferred; // number of charecer already transferred 
   int* address; // number of charcter to transfer the 4th parameter in the call of sys_req  
} DCB;



// IOCB struct
typedef struct iocb {
    pcb* process;       // the process requesting IO
    DCB* device;	    // device to be used for IO
    int op;		        // the operation code (op_code)
    char* buffer;       // the IOCB’s buffer (from sys_req)
    int* count;	        // counter to whats in the buffer (from sys_req)
    struct iocb* next;  // pointer to next IOCB in line (for list)
} IOCB;

// IOCB list struct
typedef struct ioqueue {
    IOCB* head;
    IOCB* tail;
} IOqueue;


// function prototypes go here

int com_open(int baud_rate);

int com_write (char* buf_p, int* count_p);

int IOscheduler();

void loadIOCB(pcb* proc, int code, char* buff, int* count);

void PIC(int value);

int com_read (char* buf_p, int* count_p);

int com_close (void);

int second_read ();

void second_write();

void top_handler();

void set_int(int,int);

void input_h(); // "second level input handler"

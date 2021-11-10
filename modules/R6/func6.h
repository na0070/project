// header for func6.c
#include "../R2/func.h"
#include "../mpx_supt.h"

// error codes definitions
#define PORT_NOT_OPEN        -401
#define INVALID_BUFF_ADDRESS -402
#define INVALID_COUNT        -403
#define DEVICE_BUSY          -404



// DCB codes
#define IDLE    0
#define WRITING 1
#define READING 2


// DCB struct should go here













// IOCB struct
typedef struct iocb {
    pcb* process;       // the process requesting IO
    DCB* device;	    // device to be used for IO
    int op;		        // the operation code (op_code)
    char* buffer;       // the IOCB’s buffer (from sys_req)
    int* count;	        // counter to whats in the buffer (from sys_req)
} IOCB;


// function prototypes go here

int com_write (char* buf_p, int* count_p);

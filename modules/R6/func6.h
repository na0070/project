// header for func6.c
#include "../R2/func.h"
#include "../mpx_supt.h"


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

// header for func6.c
#include "../R2/func.h"
#include "../mpx_supt.h"

// error codes definitions

#define INVALID_EFLAG        -101
#define INVALID_BRD          -102
#define PORT_AREADY_OPEN     -103



#define PORT_NOT_OPEN        -401
#define INVALID_BUFF_ADDRESS -402
#define INVALID_COUNT        -403
#define DEVICE_BUSY          -404



// DCB codes
#define IDLE     0
#define WRITING  1
#define READING  2

#define NOT_OPEN 0
#define OPEN     1


// DCB struct should go here













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

int com_open(int* eflag_p, int baud_rate);

int com_write (char* buf_p, int* count_p);

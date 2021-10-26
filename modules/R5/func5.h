#define FREE 0
#define ALLOCATED 1

// for boolean statements
#define TRUE 1
#define FALSE 0

// CMCB and LMCB structs should go here
typedef struct cmcb{
 char *Type; 
 char *beginning_address;
 int *Size;
 char *name;  
 CMBC next*;
}  CMCB;

typedef struct lmcb{
 char *Type; 
 int *Size;
}  LMCB;

// function prototypes should go here
struct list {
        CMCB *head; // dequeue this 
	CMCB *tail; // enquue affter this 
}

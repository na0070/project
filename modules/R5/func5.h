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
 struct cmcb *next;
}  CMCB;

typedef struct lmcb{
 char *Type; 
 int *Size;
}  LMCB;


struct list {
        CMCB *head; // dequeue this 
	CMCB *tail; // enquue affter this 
}

// function prototypes should go here

#define FREE 0
#define ALLOCATED 1

// for boolean statements
#define TRUE 1
#define FALSE 0

// CMCB and LMCB structs should go here
typedef struct cmcb{
 int type; 
 u32int address;
 int size;
 char *name;  
 struct cmcb *next;
 struct cmcb *prev;
}  CMCB;

typedef struct lmcb{
 int type; 
 int size;
}  LMCB;


struct list {
        CMCB *head; 
	CMCB *tail;  
};

// function prototypes should go here

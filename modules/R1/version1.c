

void version(){
 

 //sys_req(READ,DEFAULT_DEVICE,buffer,&size); // goes to polling
 
  int bsize = 30, isize = 2; // bfer size 
  char buffer[bsize];  // bufer 
  char* input ;  
  serial_print("to know each version when was published please enter the version number \n");
  
   sys_req(READ,DEFAULT_DEVICE,input,&isize); // goes to polling
   
   if ((strcmp(input,"r1") == 0) ||(strcmp(input,"R1") == 0)) { // figure out which version 
   serial_print("Published 09/03/2021 \n");
    }//end if 
 //else if ((strcmp(input,"r2") == 0) ||(strcmp(input,"R2") == 0)) { // figure out which version 
  // serial_print("Published 09/03/2021 \n"); // figure out latter 
    }//end if 
     else 
   serial_print("the input you entered is invalid  \n");
    
}// end method 


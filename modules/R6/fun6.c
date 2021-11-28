 
 #define outb (port, data) ({})
 
 
 
 // com close 
int com_close (void){
// from the document 

//     1. Ensure that the port is currently open. 
if (device.port != OPEN){
        return PORT_NOT_OPEN;}//if 
 else{    
   // 2. Clear the open indicator in the DCB. 
    device.port = CLOSE;  // indicate device is close

    device.event_flag = 0;    // device is being used

    device.currentloc = IDLE;    // device status is idle

    memset(device.userbuffer,'\0',sizeof(device.userbuffer));   // free the DCB user buffer
    memset(device.internalbuff,'\0',sizeof(device.internalbuff));   // free the DCB internal buffer
    //3. Disable the appropriate level in the PIC mask register.
  PIC(0x11); 
   // 4. Disable all interrupts in the ACC by loading zero values to the Modem Status register and the Interrupt Enable register.
   cli(); 
   // 5. Restore the original saved interrupt vector. 
   
return 0; } // else 
}// com close 

int second_write(char*	userbuffer, int *currentloc, int buffersize) {
   char  output =outb(COM1, data );
  
    //  1. If the current status is not writing, ignore the interrupt and return. 
 
  if (device.currentop != WRITING){
			
			device.userbuffer [currentloc] = output; 			
				return 0;}// if 
				else {
   int i;
  
   for(i=currentloc; i<= buffersize; i++; ){

    //2. Otherwise, if the count has not been exhausted, get the next character from the requestor's output buffer and store it in the output register. Return without signaling completion. 
    if (currentloc < buffersize ){
    device.userbuffer [i] = output;
    }// if 
    else{
    //  3. Otherwise, all characters have been transferred. Reset the status to idle. Set the event flag and return the count value. Disable write interrupts by clearing bit 1 in the interrupt enable register.
     device.currentop = IDLE; 
     device.eventflag = 0; 
     	return currentloc;  
 }//else 2     
 }//for 
 }// else     
}// second write 


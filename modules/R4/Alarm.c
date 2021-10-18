struct queue alarms = {.head = NULL, .tail = NULL};  

void createAlarm (char* message, int time) {
	
	char* currentTime = getTime();
	
		int currentSeconds = currentTime [7] *10 + currenTime [8];
		
			int currentMinutes = currentTime [4]*600 + currenTime [5]*60;
			
				int currentHours = currentTime[1]*36000 + currenTime[2]*3600; 
				
			int totalTime = currentSeconds + currentMinutes + currentHours;
			
		
		int alarmTime = totalTime + time; 
		
				pcb* pointer = setupPCB(message, SYSTEM, 3);
				
					pointer->prio = alarmTime;
					
						pointer ->state = ALARM;
						
					insertPCB (pointer);
			
					// stop here
					
			// should be moved to ch
			while (1) {
				
				char* new_currentTime = getTime();
	
					int new_currentSeconds = new_currentTime [7] *10 + new_currentTime [8];
		
						int new_currentMinutes = new_currentTime [4]*600 + new_currentTime [5]*60;
			
				int new_currentHours = new_currentTime[1]*36000 + new_currentTime[2]*3600; 
				
			int new_totalTime = new_currentSeconds + new_currentMinutes + new_currentHours;
			
		if (new_totalTime >= alarmTime ) {
				
			print (message, sizeof (message) ); 
					
				sys_req(EXIT, DEFAULT_DEVICE,NULL,NULL);
			}
				else {
					
					sys_req(IDLE, DEFAULT_DEVICE,NULL,NULL);
				}
			}

}

void checkTime () {
	
	while (1) {
		
		showqueue ("alarm"); 
			
			sys_req(IDLE, DEFAULT_DEVICE,NULL,NULL);
	}
	
}
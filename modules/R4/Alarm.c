struct alarmQueue = {.head = NULL, .tail = NULL};  

struct alarm {
	char messsage[20];
	int time;
	struct alarm* next;
	struct alarm* prev;
};
typedef struct alarm alarm;

struct alarmQueue {

	alarm* head;
	alarm* tail;
}


void createAlarm (char* message, int time) {
	
	// char* currentTime = getTime();
	
	// 	int currentSeconds = currentTime [7] *10 + currenTime [8];
		
	// 		int currentMinutes = currentTime [4]*600 + currenTime [5]*60;
			
	// 			int currentHours = currentTime[1]*36000 + currenTime[2]*3600; 
				
	// 		int totalTime = currentSeconds + currentMinutes + currentHours;
			

		//int currentTime = getCurrentTime();
		
		int alarmTime = getCurrentTime() + time; 

		alarm* newAlarm;
		newAlarm->message = message;
		newAlarm->time = alarmTime;


		if (alarmQueue->head == NULL) {				// if the queue is empty (will need to assign the new pcb as the head)

			alarmQueue->head = newAlarm;					// make the new pcb as the head and tail of the queue
			alarmQueue->tail = newAlarm;
			newAlarm->next = NULL;				// make sure the new pcb is not pointing to anything
			newAlarm->prev = NULL;
			
			loadr_pcb("alarm process", SYSTEM, NOT_SUSPENDED, 2, (u32int)checkTime);
		}
		else {								// if queue is not empty
			
			newAlarm->prev = alarmQueue->tail;			// since doing FIFO, then will simply add the new pcb to the end of the queue (making the new pcb point to the tail as "previous")
			newAlarm->prev->next = newAlarm;		// make that tail point to the new pcb as "next"
			alarmQueue->tail = newAlarm;					// set the new pcb as the new tail of the queue
			newAlarm->next = NULL;				// make sure the new pcb does not point to anything next
			
		}

		
		
				// pcb* pointer = setupPCB(message, SYSTEM, 3);
				
				// 	pointer->prio = alarmTime;
					
				// 		pointer ->state = ALARM;
						
				// 	insertPCB (pointer);
			
					// stop here
					
			// should be moved to ch
			
while (1) {
				
				char* new_currentTime = getTime();
	
					int new_currentSeconds = new_currentTime [7] *10 + new_currentTime [8];
		
						int new_currentMinutes = new_currentTime [4]*600 + new_currentTime [5]*60;
			
				int new_currentHours = new_currentTime[1]*36000 + new_currentTime[2]*3600; 
				
			int new_totalTime = new_currentSeconds + new_currentMinutes + new_currentHours;
			
		
			}
}

void checkTime () {
	
	while (1) {
		
		int time = getCurrentTime();
		alarm* pntr = alarmQueue.head;

		if (pntr == NULL)
			sys_req(EXIT, DEFAULT_DEVICE,NULL,NULL);

		else {

			while (pntr != NULL) {

				if (pntr->time >= time) {

					print(pntr->message,sizeof(pntr->message));
					print("\n",1);

					// remove alarm from the queue and edit connections
				}



				pntr = pntr->next;
			}


		}

		
			
			sys_req(IDLE, DEFAULT_DEVICE,NULL,NULL);
	}
	
}

int getCurrentTime() {

	char* currentTime = getTime();
	
		int currentSeconds = atoi(currentTime [6]) *10 + atoi(currentTime [7]);
		
			int currentMinutes = atoi(currentTime [3])*600 + atoi(currentTime [4])*60;
			
				int currentHours = atoi(currentTime[0])*36000 + atoi(currentTime[1])*3600; 
				
			return currentSeconds + currentMinutes + currentHours;
}
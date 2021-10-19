#include "Alarm.h"
#include "../mpx_supt.h"
#include "../R1/getandsettime.h"
#include "../R2/func.h"
#include <string.h>

struct alarmQueue Queue = {.head = NULL, .tail = NULL};  


void createAlarm (char* message, int time) {
	
	// char* currentTime = getTime();
	
	// 	int currentSeconds = currentTime [7] *10 + currenTime [8];
		
	// 		int currentMinutes = currentTime [4]*600 + currenTime [5]*60;
			
	// 			int currentHours = currentTime[1]*36000 + currenTime[2]*3600; 
				
	// 		int totalTime = currentSeconds + currentMinutes + currentHours;
			

		//int currentTime = getCurrentTime();
		
		int alarmTime = getCurrentTime(); 
		struct alarmQueue* Q = &Queue;

		// alarm newAlarm;

		// strcpy(newAlarm.message,message);
		// newAlarm.time = alarmTime;

		// alarm* newAlarmPtr = &newAlarm;
		alarm* newAlarmPtr = (alarm*)sys_alloc_mem(sizeof(alarm));

		strcpy(newAlarmPtr->message,message);
		newAlarmPtr->time = alarmTime;
		newAlarmPtr->delay = time;


		//  inserting the new alarm

		if (Q->head == NULL) {				// if the queue is empty (will need to assign the new alarm as the head)

			Q->head = newAlarmPtr;					// make the new alarm as the head and tail of the queue
			Q->tail = newAlarmPtr;
			newAlarmPtr->next = NULL;				// make sure the new alarm is not pointing to anything
			newAlarmPtr->prev = NULL;
			
		}
		else {								// if queue is not empty
			
			newAlarmPtr->prev = Q->tail;			// since doing FIFO, then will simply add the new alarm to the end of the queue (making the new alarm point to the tail as "previous")
			newAlarmPtr->prev->next = newAlarmPtr;		// make that tail point to the new alarm as "next"
			Q->tail = newAlarmPtr;					// set the new alarm as the new tail of the queue
			newAlarmPtr->next = NULL;				// make sure the new alarm does not point to anything next
			
		}

}

void checkTime() {
	struct alarmQueue* Q = &Queue;
	
	while (1) {
		
		int time = getCurrentTime();

		alarm* pntr = Q->head;

		if (pntr == NULL) {

			print("No alarms exist\n",16);
		}

		else {

			while (pntr != NULL) {

				if (time >= pntr->time + pntr->delay) {

					// print the alarm message
					print("ALARM: \"",9);
					print(pntr->message,sizeof(pntr->message));
					print("\"\n",2);


					if (pntr == Q->head) {								// if the alarm is the head of the queue it is in
						Q->head = pntr->next;							// make the head become the next alarm in line
						pntr->next->prev = NULL;						// make that new head point to nothing as "previous"
						//pntr->next = NULL;								// make the removed alarm point to nothing next
						
					}
					
					//trying to remove tail
					else if (pntr == Q->tail) {								// if the alarm is the tail of the queue
						
						Q->tail = pntr->prev;							// make the previous alarm as the new tail 
						pntr->prev->next = NULL;						// make that new tail not point to anything next

						pntr->prev = NULL;								// make the removed alarm point to nothing else
						//pntr->next = NULL;
					
					}
					else {
						// removing from the middle
						pntr->prev->next = pntr->next;            // instead of A pointing to B, A pointing to C
							pntr->next->prev = pntr->prev; 		// instead of C pointing to B, C pointing to A
							
							//pntr->next = NULL; 							// unlink next pointer of B "C"
						pntr->prev = NULL;								// unlink previous pointer of B "A"
				

						
					}

				}

				pntr = pntr->next;
			}


		}
	
			sys_req(IDLE, DEFAULT_DEVICE,NULL,NULL);
	}
	
}

int getCurrentTime() {

	int currentTime[6];
	get_Time(currentTime);
	
		int currentSeconds = currentTime [4] *10 + currentTime [5];
		
			int currentMinutes = currentTime [2]*600 + currentTime [3]*60;
			
				int currentHours = currentTime[0]*36000 + currentTime[1]*3600; 
				
			return currentSeconds + currentMinutes + currentHours;
}
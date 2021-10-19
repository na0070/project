
struct alarm {
	char message[20];
	int time;
	int delay;
	struct alarm* next;
	struct alarm* prev;
};
typedef struct alarm alarm;

struct alarmQueue {

	alarm* head;
	alarm* tail;
};

void createAlarm (char* message, int time);

void checkTime();

int getCurrentTime();
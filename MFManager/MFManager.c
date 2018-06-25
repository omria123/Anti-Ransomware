#include "manager_communication.h"
#include <windows.h>
#include <stdio.h>
#include <fltUser.h>

#define CHANGES_LENGTH 15 //TODO:change length


static unsigned long rid = 0;

inline void manager__clean_handling(MINIFITLER_REPORT *message)
{
	free(message);
}

DWORD WINAPI manager__handle_request(MINIFITLER_REPORT *message) 
{
	int todo = 0;
	char delay[5] = "2";
	char *p = delay;




	switch(todo)
	{
		case 0: //pass
			manager_communication__reply_message(message, "0", 1);
			break;
		case 1: //stop the request
			manager_communication__reply_message(message, "1", 1);
			break;
		case 2: //delay
			memcpy(p + 1, &rid, 4);
			rid++;
			manager_communication__reply_message(message, delay, 5);
			break;

		case 3: //ask for IRP
			manager_communication__reply_message(message, "4", 1);
			break;
	}

	printf("address of path: %p\n", message->report.path);
	printf("%s has been modified by process - PID form %c \n" ,message->report.path, message->report.mode);
	printf("it writed %ul bytes to the buffer in %llu\n\n\n", message->report.operation_parameters.write.buffer_len, message->report.operation_parameters.write.offset);


	manager__clean_handling(message);
	free(message);
	return 0;
}




int main()
{
	HANDLE thread;

	if(!manager_communication__init_port())
	{
		printf("problem with initiating port, closing.......\n");
		return 1;
	}

	while(1)
	{
		int status;
		PARSED_REPORT *report = malloc(sizeof(MINIFITLER_REPORT));

		status = manager_communication__get_report(report);
		if(status)
			thread = CreateThread(NULL, 0, manager__handle_request, report, 0, NULL);

	}
	return 0;

}







#include "parser.h"
#include "manager_communication.h"
#include <windows.h>
#include <stdio.h>

static unsigned long rid = 0;

inline void manager__clean_handling(MINIFITLER_REPORT *message, PARSED_REPORT *parsed)
{
	free(message);
	free(parsed-4);
}

DWORD WINAPI manager__handle_request(MINIFITLER_REPORT *message) 
{
	int todo = 0;
	PARSED_REPORT parsed;
	char alt_data[sizeof(FLT_CALLBACK_DATA)] + 1 = "2", delay[5] = "3";
	char *p = delay;



	parser__parse_report(&(message->report), &parsed);

	switch(todo)
	{
		case 0: //pass
			manager_communication__reply(message, "0", 1);
			break;
		case 1: //stop the request
			manager_communication__reply(message, "1", 1);
			break;
		case 2: // edit operation
			manager_communication__reply(message, alt_data, sizeof(FLT_CALLBACK_DATA) + 1);
			break;
		case 3: //delay
			memcpy(p + 1, &rid, 4);
			rid++;
			manager_communication__reply(message, delay, 5);
			break;
		case 4: //ask for IRP
			manager_communication__reply(message, "4", 1);
			break;
	}


	printf("%.*s has been modified by process - PID\n", parsed.path_length ,parsed.path);

	manager__clean_handling(message, &parsed);
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
		MINIFITLER_REPORT *report = malloc(sizeof(MINIFITLER_REPORT));

		status = manager_communication__get_report(&report);
		if(status)
			thread = CreateThread(NULL, 0, manager__handle_request, report, 0, NULL);

	}
	return 0;

}







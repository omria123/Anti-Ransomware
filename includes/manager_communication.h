#ifndef MANAGER_COMMUNICATION__H
#define MANAGER_COMMUNICATION__H
#define PORT_NAME "AntiRansomCommun"

#include <Fltuser.h>


//This module is the communincator of the minifilter manager to the minifilter itself



typedef struct _MINIFITLER_REPORT {
	unsigned long reply_length;
	unsigned long long message_id;
	OPREPORT data; 
} MINIFITLER_REPORT;




int  manager_communication__init_port(void);


void manager_communication__get_call();



extern HANDLE manager_communication__port_handle;


#endif


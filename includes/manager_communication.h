#ifndef MANAGER_COMMUNICATION__H
#define MANAGER_COMMUNICATION__H


#define PORT_NAME "AntiRansomCommun"



#include <Fltuser.h>


//This module is the communincator of the minifilter manager to the minifilter itself



typedef struct _OPREPORT {
	FLT_CALLBACK_DATA data;
	FLT_RELATED_OBJECTS obj;
} OPREPORT;




typedef struct _MINIFITLER_REPORT {
	OPREPORT report; 
	unsigned long reply_length;
	unsigned long long message_id;
} MINIFITLER_REPORT;




int  manager_communication__init_port( void);

int manager_communication__get_report( void*);

int manager_communication__reply_message(MINIFITLER_REPORT *, char *, int size);

void manager_communication__close_port( void);


extern HANDLE manager_communication__port_handle; //the handle to the communication port used to contact the minifilter


#endif


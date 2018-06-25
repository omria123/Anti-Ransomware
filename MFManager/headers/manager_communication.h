#ifndef MANAGER_COMMUNICATION__H
#define MANAGER_COMMUNICATION__H


#define PORT_NAME L"\\AntiRansomCommun"


#include <stdlib.h>
#include <Windows.h>
//This module is the communincator of the minifilter manager to the minifilter itself



typedef struct _WRITE_PARAMETERS {
	char buffer[1];
	unsigned long buffer_len;
	unsigned long long offset;
} WRITE_PARAMETERS;


typedef union _PARAMETERS {

	WRITE_PARAMETERS write;

} PARAMETERS;


typedef struct _PARSED_REPORT
{
	char mode; //kernel-k or user- u modes
	long current_status;
	char operation_type;//TODO: write type
	PARAMETERS operation_parameters;
	char path[_MAX_PATH];
	unsigned long path_length;

} PARSED_REPORT;





typedef struct _MINIFITLER_REPORT {
	PARSED_REPORT report; 
	unsigned long reply_length;
	unsigned long long message_id;
} MINIFITLER_REPORT;




int  manager_communication__init_port( void);

int manager_communication__get_report( void*);

int manager_communication__reply_message(MINIFITLER_REPORT *, char *, int size);

void manager_communication__close_port( void);


extern HANDLE manager_communication__port_handle; 

//the handle to the communication port used to contact the minifilter


#endif


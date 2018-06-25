#include "manager_communication.h"
#include <string.h>
#include <windows.h>
#include <fltUser.h>

HANDLE manager_communication__port_handle;


int manager_communication__init_port()
{
	//the function use the given handle to connect to the minifilter return 0 for failed and 1 for success
	

	return FilterConnectCommunicationPort(PORT_NAME,
	0, //flags
	NULL,  //information to send to the minifilter when the connection accures aka context
	0,  //length of context
	NULL, //security attributes
	&manager_communication__port_handle) == S_OK; //pointer to handler of the communication port

}




int manager_communication__get_report( void *msg)
{
	//the function fill the given pointer with the memory of the report
	HRESULT status;
	status = FilterGetMessage(manager_communication__port_handle,
	msg,
	sizeof(MINIFITLER_REPORT),
	NULL); //option for async
	return status == S_OK;
}


void manager_communication__close_port()
{
// the function will close the handle
	CloseHandle(manager_communication__port_handle);
}



int manager_communication__reply_message(MINIFITLER_REPORT *msg, char *reply, int size)
{
	//the function will reply to previous message from the minifilter
	int len = sizeof(FILTER_REPLY_HEADER);	
	PFILTER_REPLY_HEADER header = malloc(len + size);
	HRESULT status;

	header->MessageId = msg->message_id;
	header->Status =  0;//Status Success
	memcpy(header + len, reply, size);
	status = FilterReplyMessage(manager_communication__port_handle, header, len + size);
	
	free(header);
	return status == S_OK;
}




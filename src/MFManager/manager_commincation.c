#include "manager_communication"
#include <string.h>


HANDLE manager_communication__port_handle;



int manager_communication__init_port()
{
	//the function use the given handle to connect to the minifilter return 0 for failed and 1 for success
	HRESULT status;
	FilterConnectCommunicationPort(PORT_NAME,
	0, //flags
	NULL,  //information to send to the minifilter when the connection accures
	NULL, //security attributes
	&manager_communication__port_handle); //pointer to handler of the communication port
	return status == S_OK;
}




int manager_communication__get_report( void *msg)
{
	//the function fill the given pointer with the memory of the report
	HRESULT status;
	status = FilterGetMessage(manager_communication__port_handle,
	msg,
	sizeof(MINIFILTER_REPORT),
	NULL); //option for async
	return status == S_OK;
}


void manager_communication__close_port()
{
// the function will close the handle
	CloseHandle(manager_communication__port_handle);
}



int manager_communication__reply_message(MINIFILTER_REPORT *msg, char *reply, int size)
{
	//the function will reply to previous message from the minifilter
	int len = sizeof(FILTER_REPLY_HEADER);	
	PFILTER_REPLY_HEADER header = malloc(len + size);
	HRESULT status;

	header.MessageID = msg.message_id;
	header.Status =  STATUS_SUCCESS;
	memcpy(header + FILTER_REPLY_HEADER, reply, size);
	status = FilterReplyMessage(manager_communication__port_handle, header, len + size);
	
	free(header);
	return status == S_OK;
}




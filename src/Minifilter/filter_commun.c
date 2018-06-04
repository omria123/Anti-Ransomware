#include "filter_commun.h"
#include "minifilter.h"
#include <windows.h>

PFLT_PORT  filter_commun__server_port = NULL, filter_commun__client_port = NULL;


NTSTATUS filter_commun__register_communication_port()
{
	PSECURITY_DESCRIPTOR sd;
    OBJECT_ATTRIBUTES oa;
    UNICODE_STRING unicode_str;
    NTSTATUS status = STATUS_SUCCESS;

    status  = FltBuildDefaultSecurityDescriptor( &sd,
                                                 FLT_PORT_ALL_ACCESS );

    if (!NT_SUCCESS( status )) {
        return status;
    }

    RtlInitUnicodeString( &uniString, PORT_NAME);

    InitializeObjectAttributes( &oa,
                                &unicode_str,
                                OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE,
                                NULL,
                                sd );



    status = FltCreateCommunicationPort(registration__filter,
                                         &filter_commun__server_port,
                                         &oa,
                                         NULL, //Server Cookie
                                         filter_commun__accept_connection, // handler for accepting connection
                                         filter_commun__disconnect, // handler for disconnecting a client
                                         filter_commun__recieve_message, // handler for receiving messages
                                         1 ); // Max Connections

    FltFreeSecurityDescriptor( sd );

    if (!NT_SUCCESS( status )) {
        return status;
    }

}



NTSTATUS filter_commun__accept_connection(
    _In_ PFLT_PORT ClientPort,
    _In_ PVOID ServerPortCookie,
    _In_reads_bytes_(SizeOfContext) PVOID ConnectionContext,
    _In_ ULONG SizeOfContext,
    _Flt_ConnectionCookie_Outptr_ PVOID *ConnectionCookie
    )
{
	UNREFERENCED_PARAMETER( ServerPortCookie );
    UNREFERENCED_PARAMETER( ConnectionContext );
    UNREFERENCED_PARAMETER( SizeOfContext);
    UNREFERENCED_PARAMETER( ConnectionCookie );

    if(filter_commun__client_port == NULL)
		cfilter_commun__client_port = ClientPort;

    return STATUS_SUCCESS;
}


void filter_commun__disconnect( _In_opt_ PVOID ConnectionCookie)
{
	UNREFERENCED_PARAMETER(ConnectionCookie);
	close(communication__client_port)
    FltCloseClientPort( registration__filter, &communication__client_port);

    communication__client_port = NULL;

	return STATUS_SUCCESS;
}


NTSTATUS filter_commun__send_message(char * msg, char *reply, char type)
{
	/*
	The function sends to the client process the message and wait for response that is returned in reply
	type determine how long the function should wait 0 for forever and 1 for short period of time
	*/
	unsigned long m_size = MAX_REPORT , r_size=MAX_REPLY;
	int timeout = WAIT_INDEFINITELY;
	if(type)
		timeout = QUICK_TIME_RESPONSE;
	return FltSendMessage(registration__filter, filter_commun__client_port, msg, &m_size, reply, &r_size, &timeout);
}

typedef struct _MSGINFO {
	unsigned long nid;
	NTSTATUS ret_status;
} MSGINFO;


NTSTATUS filter_commun__recieve_message  (
      IN PVOID PortCookie,
      IN PVOID InputBuffer OPTIONAL,
      IN ULONG InputBufferLength,
      OUT PVOID OutputBuffer OPTIONAL,
      IN ULONG OutputBufferLength,
      OUT PULONG ReturnOutputBufferLength
      )
{
	/*
	*This function receives new message from the user-mode application.
	*The message will contain the information needed to end one of the pending I/O operation after the user-mode application finished process it
	*the function will parse the information and then pass it on in order to complete the opertaion
	*/
	MSGINFO input;
	if(InputBufferLength !=sizeof(MSGINFO))
		; //optional - return error code
	copyMemory(&input, InputBuffer, sizeof(MSGINFO));
	minifilter__finish_operation(input.ret_status, input.nid);
	return STATUS_SUCCESS;
	
}





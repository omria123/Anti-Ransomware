#include <fltkernel.h>
#include "commun_filter.h"

NTSTATUS register_communication_port(PFLT_FILTER filter)
{
	NTSTATUS status;
	PSECURITY_DESCRIPTOR sd;
	OBJECT_ATTRIBUTES oa;
	UNICODE_STRING unistring;


	status = FltBuildDefaultSecurityDescriptor( &sd, FLT_PORT_ALL_ACCESS );
	if(!NT_SUCCESS(status))
		return status;
	
	RtlInitUnicodeString(&unistring, PORT_NAME); // port name to unicode


	InitializeObjectAttributes(&oa,
								&unistring,
								OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE,//attributes
								NULL,
								&sd);


	status = FltCreateCommunicationPort(filter,
	                                   	&communication__server_port,
                                        &oa, //
                                        NULL, //ServerCookie
                                        NULL, //Connect Handler
                                        NULL, //Disconnect Handler
                                        NULL, //Message Received Handler
                                        1 ); //Maximum Connection


    FltFreeSecurityDescriptor( sd );
   	return status;
}






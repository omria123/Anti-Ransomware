#include "commun_filter.h"


PFLT_PORT  communication__server_port;



NTSTATUS filter_commun__register_communication_port(PFLT_FILTER flt)
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



    status = FltCreateCommunicationPort(flt,
                                         &communication__server_port,
                                         &oa,
                                         NULL, //Server Cookie
                                         , // handler for accepting connection
                                         , // handler for disconnecting a client
                                         , // handler for receiving messages
                                         1 ); // Max Connections

    FltFreeSecurityDescriptor( sd );

    if (!NT_SUCCESS( status )) {
        return status;
    }

}


// TODO: write a handler for accepting conneciton
// TODO: write a handler for disconnecting connection
// TODO: write a handler for accepting a connection



// TODO: write client side connection
// TODO: create generic api for both sides of communication




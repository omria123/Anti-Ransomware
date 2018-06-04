#ifndef FILTER_COMMUN__H
#define FILTER_COMMUN__H


#include <fltkernel.h>

#define PORT_NAME "AntiRansomCommun"
#define MAX_REPLY_LENGTH 
#define REPORT_LENGTH sizeof(FLT_CALLBACK_DATA) + sizeof(FLT_RELATED_OBJECTS)
#define WAIT_INDEFINITELY 0
#define QUICK_TIME_RESPONSE 10 

//To be changed ^^^^^^^^^^^^^^^ the value wasn't checked and may be misleading 




NTSTATUS filter_commun__register_communication_port(void);

NTSTATUS filter_commun__accept_connection(
    _In_ PFLT_PORT ClientPort,
    _In_ PVOID ServerPortCookie,
    _In_reads_bytes_(SizeOfContext) PVOID ConnectionContext,
    _In_ ULONG SizeOfContext,
    _Flt_ConnectionCookie_Outptr_ PVOID *ConnectionCookie
    );

void filter_commun__disconnect( _In_opt_ PVOID ConnectionCookie);

extern PFLT_PORT  filter_commun__server_port;

extern PFLT_PORT filter_commun__client_port;


#endif

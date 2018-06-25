#ifndef FILTER_COMMUN__H
#define FILTER_COMMUN__H

#include <fltkernel.h>

#define PORT_NAME L"\\AntiRansomCommun"
#define MAX_REPLY_LENGTH 10
#define REPORT_LENGTH sizeof(PARSED_REPORT)
#define WAIT_INDEFINITELY 0
#define QUICK_TIME_RESPONSE 100
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

NTSTATUS filter_commun__send_message(void * msg, char *reply, char type);

NTSTATUS filter_commun__receive_message(
	IN PVOID PortCookie,
	IN PVOID InputBuffer OPTIONAL,
	IN ULONG InputBufferLength,
	OUT PVOID OutputBuffer OPTIONAL,
	IN ULONG OutputBufferLength,
	OUT PULONG ReturnOutputBufferLength
);

#endif

#include <fltkernel.h>
#include "minifilter.h"
#include "commun_filter.h"



NTSTATUS close_filter(FLT_FILTER_UNLOAD_FLAGS flags)
{

	if (flags != FLTFL_FILTER_UNLOAD_MANDATORY)
		return STATUS_FLT_DO_NOT_DETACH;
	
	
	if (registaraoion__filter != NULL) //if filter has been registered
		FltUnregisterFilter(registration__filter);
		
		
	if (communication__server_port) //if communication was set
		FltCloseCommunicationPort(communication__server_port );
	
	return STATUS_SUCCESS;
}



NTSTATUS DriverEntry (_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath)
{
	
	NSTATUS status = STATUS_SUCCESS;
	
	//register the minifilter with Fliter Manager
	
	status = FltRegisterFilter(DriverObject, &registration__detailes ,&registration__filter);
	
	if(!NT_SUCCESS(status))
	{
		//output the reason - optional
		close_filter(status);
		return status;
	}
	
	
	//Register the minifilter communication
	status = filter_commun__register_communication_port(registration__filter);
	
	
	//end of entry: 
	status = FltStartFiltering(registaraoion__filter);
	if (!NT_SUCCESS(status))
	{
		close_filter(status);
		return status;
	}
	
}




FLT_PREOP_CALLBACK_STATUS write_preoperation_callback(
	_Inout_ PFLT_CALLBACK_DATA Data;
	_In_ PCFLT_RELATED_OBJECTS FltObjects,
    _Flt_CompletionContext_Outptr_ PVOID *CompletionContext)	
{
	/*
	This function will receive each and every writing opertaion before it accurs.
	
	The function recieves three parameters:
	1. Data - the information about the write request, this argument is passed through all the file system filters in the system.
	each filter has the option to alter the request by editing this argument, and the option to prevent it from happening.
	
	2. FltObjects - pointers to some 
	*/
	char response[MAX_MESSAGE];




	filter_commun__report_operation(Data, response); //TODO: write this function





	*CompletionContext = NULL;
}






#include <fltkernel.h>
#include "minifilter.h"



NTSTATUS close_filter(FLT_FILTER_UNLOAD_FLAGS flags)
{
	if (flags != FLTFL_FILTER_UNLOAD_MANDATORY)
		return STATUS_FLT_DO_NOT_DETACH;
	
	
	if (Registaraoion__filter != NULL) //if filter has been registered
		FltUnregisterFilter(Registaraoion__filter);
		
		
	if (Communication__server_port) //if communication was set
		FltCloseCommunicationPort(Communication__server_port );
	
	return STATUS_SUCCESS;
	
}



NTSTATUS DriverEntry (_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath)
{
	
	NSTATUS status = STATUS_SUCCESS;
	
	
	
	
	//register the minifilter with Fliter Manager
	
	status = FltRegisterFilter(DriverObject, &Registration__detailes ,&Registaraoion__filter);
	
	if(!NT_SUCCESS(status))
	{
		//output the reason - optional
		return close_filter(status);
	}
	
	
	//Register the minifilter communication
	
	status = 
	
	
	
	
	
	
	//end of entry: 
	status = FltStartFiltering(Registaraoion__filter);
	if (!NT_SUCCESS(status))
		return close_filter(status);
	
}




FLT_PREOP_CALLBACK_STATUS write_preoperation_callback(
	_Inout_ PFLT_CALLBACK_DATA Data;
	_In_ PCFLT_RELATED_OBJECTS FltObjects,
    _Flt_CompletionContext_Outptr_ PVOID *CompletionContext)	
{
	//write pre-operation function
	
	
}






#include <fltkernel.h>
#include "minifilter.h"
#include "commun_filter.h"
#include <windows.h>
#include "list.h"



static LISTOP minifilter__list_operations = NULL;

NTSTATUS close_filter(FLT_FILTER_UNLOAD_FLAGS flags)
{

	if (flags != FLTFL_FILTER_UNLOAD_MANDATORY)
		return STATUS_FLT_DO_NOT_DETACH;
	
	if (registration__filter != NULL) //if filter has been registered
		FltUnregisterFilter(registration__filter);
		
		
	if (filter_commun__server_port) //if communication was set
		FltCloseCommunicationPort(filter_commun__server_port );
	
	return STATUS_SUCCESS;
}



NTSTATUS DriverEntry (_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath)
{
	UNREFERENCED_PARAMETER(RegistryPath);
	
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
	status = FltStartFiltering(registration__filter);
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
	
	2. FltObjects - pointer to struct which hold more information about the request

	3. CompletionContext - a information which can be transfered from the pre-operation callback to the corresponding post-operation callback of the minifilter
	
	It will send the first two arguments to the filter 
	*/

	char response[MAX_MESSAGE];
	int response_code, status, data_size =sizeof(FLT_CALLBACK_DAT);
	FLT_PREOP_CALLBACK_STATUS ret_status;
	FLT_CALLBACK_DATA alternative_data;
	PFLT_CALLBACK_DATA tmp;
	unsigned long node_id; 
		
	status = minifilter__report_operation(Data, FltObjects, response); //TODO: write this function

	if(status != 0) //if the communication failed there is no need in delaying the request and slow down the computer so let pass
		response_code = 0;
	else
		minfilter__parse_response(response, &response_code, &alternative_data, &node_id);

	//TODO : maybe change the response_code from 0,1,2,3..... to something else 
	switch(response_code) {
		case 1: //stop the request
    		Data->IoStatus.Information = 0;
			Data.IoStatus.Status = STATUS_CANCELLED ;
			ret_status = FLT_PREOP_COMPLETE;
			break;
		case 2: //editing the request		
			CopyMemory(Data, alternative_data, data_size);
			FltSetCallbackDataDirty(Data);
			break;
		case 3: //Delay the request - possible only for IRP
			tmp = malloc(data_size);
			copyMemory(tmp, Data, data_size);
			list__add(minifilter__list_operations, tmp, node_id);
			ret_status = FLT_PREOP_PENDING;
			break;
		case 4: //ask for IRP instead
			ret_status = FLT_PREOP_DISALLOW_FASTIO 
			break;
		default: //pass the request further (instead of case 0)
			ret_status = FLT_PREOP_SUCCESS_NO_CALLBACK;
			break;
	}

	*CompletionContext = NULL; //this minifilter doesnt have write post-operation function so it required to be NULL
	return ret_status;
}



int minifilter__report_operation(PFLT_CALLBACK_DATA Data, PCFLT_RELATED_OBJECTS FltObjects, char* response)
{
	NTSTATUS status;
	unsigned long d_size = sizeof(FLT_CALLBACK_DATA), o_size = sizeof (FLT_RELATED_OBJECTS);
	char * request = malloc(d_size + r_size);

	CopyMemory(request, Data, d_size);
	CopyMemory(request+d_size, FltObjects, o_size);
	
	status = filter_commun__send_message(request, d_size + r_size, response, MAX_MESSAGE, 1);

	free(request);

	switch(status) {
		case STATUS_TIMEOUT: //timeout happend
			return 1;
		case STATUS_PORT_DISCONNECTED:
			return 2; //the port is disconnected
		case STATUS_INSUFFICIENT_RESOURCES:
			return 3; //internal error 
		default: //alternative to STATUS_SUCCESS 
			return 0;			
	}
}



void minfilter__parse_response(char *response, int *code, PFLT_CALLBACK_DATA replacement,
 unsigned long  *data_id)
{


}


void minifilter__finish_operation(NTSTATUS ret_status, unsigned long data_identifier)
{
	PFLT_CALLBACK_DATA data;
	OPLIST * node;

	//pull fromn list
	node = list__find(minifilter__list_operations, data_identifier);
	data = node->data;
	FltCompletePendedPreOperation(*data, ret_status, NULL);
	//clear data and remove from list
	free(data);
	minifilter__list_operations = list__remove(minifilter__list_operations, data_identifier);
	

}



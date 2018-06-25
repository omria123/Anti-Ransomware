#include <fltkernel.h>
#include <string.h>


#include "minifilter.h"
#include "filter_commun.h"
#include "list.h"
#include "parser.h"
#include "kalloc.h"

static OPLIST *minifilter__list_operations = NULL;


NTSTATUS PfltInstanceSetupCallback(
	PCFLT_RELATED_OBJECTS FltObjects,
	FLT_INSTANCE_SETUP_FLAGS Flags,
	DEVICE_TYPE VolumeDeviceType,
	FLT_FILESYSTEM_TYPE VolumeFilesystemType
)
{
	UNREFERENCED_PARAMETER(FltObjects);
	UNREFERENCED_PARAMETER(VolumeFilesystemType);
	
	if (Flags & FLTFL_INSTANCE_SETUP_MANUAL_ATTACHMENT)
		return STATUS_SUCCESS;
	if (VolumeDeviceType != FILE_DEVICE_DISK_FILE_SYSTEM )
		return STATUS_FLT_DO_NOT_ATTACH;
	
	
	return STATUS_SUCCESS;

}



NTSTATUS minifilter__close_filter(FLT_FILTER_UNLOAD_FLAGS flags)
{
	
	if (registration__filter != NULL) //if filter has been registered
		FltUnregisterFilter(registration__filter);
		
		
	if (filter_commun__server_port!=NULL ) //if communication was set
		FltCloseCommunicationPort(filter_commun__server_port );
	
	return STATUS_SUCCESS;
}



NTSTATUS DriverEntry (_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath)
{
	UNREFERENCED_PARAMETER(RegistryPath);
	
	NTSTATUS status = STATUS_SUCCESS;
	
	//register the minifilter with Fliter Manager
	
	status = FltRegisterFilter(DriverObject, &registration__detailes ,&registration__filter);
	
	if(!NT_SUCCESS(status))
	{
		//output the reason - optional
		minifilter__close_filter(FLTFL_FILTER_UNLOAD_MANDATORY);
		return status;
	}
	
	
	//Register the minifilter communication
	status = filter_commun__register_communication_port();



	//Initialize dynamic allocating
	kalloc__init();

	//end of entry: 
	status = FltStartFiltering(registration__filter);
	if (!NT_SUCCESS(status))
	{
		minifilter__close_filter(FLTFL_FILTER_UNLOAD_MANDATORY);
		return status;
	}



	return status;
}




FLT_PREOP_CALLBACK_STATUS write_preoperation_callback(
	_Inout_ PFLT_CALLBACK_DATA Data,
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

	char response[MAX_REPLY_LENGTH];

	int response_code, status;
	FLT_PREOP_CALLBACK_STATUS ret_status;
	unsigned long node_id=0; 
	
	if (filter_commun__client_port == NULL)
	{//if no connection yet
		return FLT_PREOP_SUCCESS_NO_CALLBACK;
	}
	if (!NT_SUCCESS(Data->IoStatus.Status))
		return FLT_PREOP_SUCCESS_NO_CALLBACK;



	status = minifilter__report_operation(Data, FltObjects, response); 

	if(status != 0) //if the communication failed there is no need in delaying the request and slow down the computer so let pass
		response_code = 0;
	else
		minfilter__parse_response(response, &response_code, &node_id);

	//TODO : maybe change the response_code from 0,1,2,3..... to something else 
	switch(response_code) {
		case 1: //stop the request
    		Data->IoStatus.Information = 0;
			Data->IoStatus.Status = STATUS_CANCELLED ;
			ret_status = FLT_PREOP_COMPLETE;
			break;
		case 2: //Delay the request - possible only for IRP
			minifilter__list_operations = list__add(minifilter__list_operations, Data, node_id);
			ret_status = FLT_PREOP_PENDING;
			break;
		case 3: //ask for IRP instead
			ret_status = FLT_PREOP_DISALLOW_FASTIO;
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
	PARSED_REPORT parsed;

	parser__parse_report(Data, FltObjects, &parsed);

	status = filter_commun__send_message(&parsed, response, 1);


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



void minfilter__parse_response(char *response, int *code,
 unsigned long  *data_id)
{
	*code = (*response) - '0';
	if(*code == 3)
		memcpy(data_id, response + 1, 4);

	return;
}


void minifilter__finish_operation(NTSTATUS ret_status, unsigned long data_identifier)
{
	OPLIST * node;
	
	//pull fromn list
	node = list__find(minifilter__list_operations, data_identifier);
	FltCompletePendedPreOperation(node->data, ret_status, NULL);
	//clear data and remove from list
	minifilter__list_operations = list__remove(minifilter__list_operations, data_identifier);
}



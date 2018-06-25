#ifndef MINIFILTER__H
#define MINIFILTER__H

#include <fltkernel.h>




// stating the minimum system os version


extern PFLT_FILTER registration__filter;
extern FLT_REGISTRATION registration__detailes;



//minifilter aid operations: 
NTSTATUS minifilter__close_filter(FLT_FILTER_UNLOAD_FLAGS); //cleaning the Minifilter


//filter operations callbacks:
FLT_PREOP_CALLBACK_STATUS write_preoperation_callback(
	_Inout_ PFLT_CALLBACK_DATA Data,
	_In_ PCFLT_RELATED_OBJECTS FltObjects,
    _Flt_CompletionContext_Outptr_ PVOID *CompletionContext); //the function is set as handler whenever write_IRP is sent


NTSTATUS PfltInstanceSetupCallback(
	PCFLT_RELATED_OBJECTS FltObjects,
	FLT_INSTANCE_SETUP_FLAGS Flags,
	DEVICE_TYPE VolumeDeviceType,
	FLT_FILESYSTEM_TYPE VolumeFilesystemType
);


int minifilter__report_operation(PFLT_CALLBACK_DATA Data, PCFLT_RELATED_OBJECTS FltObjects, char* response);

void minfilter__parse_response(char *response, int *code,
	unsigned long  *data_id);

void minifilter__finish_operation(NTSTATUS ret_status, unsigned long data_identifier);



#endif



#include "parser.h"
#include <Ntifs.h>
#define MAX_FILE_PATH_INFO 
/*    ULONG                   Length;
    ULONG POINTER_ALIGNMENT Key;
    LARGE_INTEGER           ByteOffset;
    PVOID                   WriteBuffer;
    PMDL                    MdlAddress;*/
/*
Cached operations - operations which will read data from the file system and store it in fast-fetch memory

Buffered operations - think of it like a bus, the bus is taking only if it has X people on it otherwise it whouldn't drive
it takes the operations to the file system in chunks instead of in one byte in a time so it can reduce latency

Difference - Cache is used for reissuing the same data again and again and Buffer is used for doing operations in chunks instead of one by one
it can also accured once for each data unit and still be useful contrasted to cahcing
*/

int parser__parse_report(OPREPORT *report, PARSED_REPORT *final )
// the function parse the first argument into the second
{

	NTSTATUS status; 
	HANDLE file_handle;
	IO_STATUS_BLOCK info_status;
	PFLT_RELATED_OBJECTS obj = report->obj;
	PFLT_CALLBACK_DATA data = report->data;
	PFILE_NAME_INFORMATION name_info;
	ULONG length, top_level;


	final->mode =(KernelMode == data->RequestorMode) ? 'k' :'l'; //kernel mode or user mode
	final->current_status = (data->IoStatus).Status; //the current status of the opertaion
	switch((data->Iopb.MajorFunction)) { //the operation type 
	case IRP_MJ_WRITE:
		final->operation_type = 'w';
		break; 

	};

	final->operation_parameters = data->Iopb.Parameters;


	ObOpenObjectByPointer(obj->FileObject, 0,NULL,0,IoFileObjectType,UserMode,  &file_handle);

	

	length = parser__get_objfile_length(&file_handle);

	if (length == 0)
	return 1;

	name_info = malloc(length + sizeof(length));

	NtQueryInformationFile(file_handle, &info_status, &name_info, sizeof name_info, FileNameInformation);
	if(info_status.Status != STATUS_SUCCESS) 
	{	
		free(name_info);
		return 1; //the file is on volume that isn't mounted 
	}

	final->path_length = length;
	final->path = name_info.FileName; // get file full path.
}


ULONG parser__get_objfile_length(PHANDLE h)
{
	IO_STATUS_BLOCK info_status;
	FILE_NAME_INFORMATION info;

	NtQueryInformationFile(*h, &info_status, &info, sizeof info, FileNameInformation);
	if(info_status.Status == STATUS_SUCCESS) 
		return info.FileNameLength;
	return 0;
}







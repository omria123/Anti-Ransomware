#include "parser.h"
#include <fltKernel.h>
#define MAX_FILE_PATH_INFO 
/*	
	ULONG                   Length;
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

int parser__parse_report(PFLT_CALLBACK_DATA op_data ,PCFLT_RELATED_OBJECTS obj, PARSED_REPORT *final )
// the function parse the first argument into the second
{ 
	//mode, status, Major Function, Parameters, target Object file(from it i got path, )
	NTSTATUS status; 

	final->mode =(KernelMode == op_data->RequestorMode) ? 'k' :'l'; //kernel mode or user mode
	final->current_status = (op_data->IoStatus).Status; //the current status of the opertaion
	switch(op_data->Iopb->MajorFunction) { //the operation type 
	case IRP_MJ_WRITE:
		final->operation_type = 'w';
		break; 
	};

	parser__parse_parameters(op_data->Iopb, &(final->operation_parameters));


	status = FltQueryInformationFile(obj->Instance, obj->FileObject, &(final->path_length), _MAX_PATH + sizeof (ULONG), FileNameInformation, NULL);
	
	if(status != STATUS_SUCCESS) 
		return 1; //the file is on volume that isn't mounted 
	return 0;
}
void parser__parse_parameters(PFLT_IO_PARAMETER_BLOCK params_block, PARAMETERS *parsed_params)
{
	FLT_PARAMETERS org_params = params_block->Parameters;
	switch (params_block->MajorFunction) {
	case IRP_MJ_WRITE:
		parsed_params->write.buffer_len = org_params.Write.Length;
		//memcpy(parsed_params->write.buffer, org_params.Write.WriteBuffer, org_params.Write.Length);
		parsed_params->write.offset = org_params.Write.ByteOffset.QuadPart;
		break;
	};

}





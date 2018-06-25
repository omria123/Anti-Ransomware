#ifndef PARSER__H
#define PARSER__H

#include <stdlib.h>
#include <fltKernel.h>

typedef struct _WRITE_PARAMETERS {
	char buffer[1];
	unsigned long buffer_len;
	unsigned long long offset;
} WRITE_PARAMETERS;


typedef union _PARAMETERS{

	WRITE_PARAMETERS write;

} PARAMETERS;

typedef struct _PARSED_REPORT 
{
	char mode; //kernel-k or user- u modes
	NTSTATUS current_status;
	char operation_type;//TODO: write type
	PARAMETERS operation_parameters;
	unsigned long path_length;
	char path[_MAX_PATH];
	
} PARSED_REPORT;


void parser__parse_parameters(PFLT_IO_PARAMETER_BLOCK params_block, PARAMETERS *parsed_params);

int parser__parse_report( PFLT_CALLBACK_DATA data , PCFLT_RELATED_OBJECTS obj, PARSED_REPORT *final);


ULONG parser__get_objfile_length(PFLT_INSTANCE inst, PFILE_OBJECT of);

#endif




#ifndef PARSER__H
#define PARSER__H


typedef struct _PARSED_REPORT 
{
	char mode; //kernel-k or user- u modes
	NTSTATUS current_status;
	operation_type;//TODO: write type
	FLT_PARAMETERS operation_parameters;
} PARSED_REPORT;



int parser__parse_report( OPREPORT * report, PARSED_REPORT *final);


ULONG parser__get_objfile_length(PFLT_INSTANCE inst, PFILE_OBJECT of);

#endif




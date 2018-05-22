#include <fltkernel.h>
#include "minifilter.h"


const PFLT_FILTER Registaraoion__filter; 

const FLT_CONTEXT_REGISTRATION context[] = {
	{ 
		FLT_TRANSACTION_CONTEXT,
		0,
		NULL, 
		sizeof(FLT_CONTEXT_REGISTRATION)
		'damn'
		NULL,
		NULL,
		NULL},
		
		
	{FLT_CONTEXT_END}
};

const FLT_OPERATION_REGISTRATION operations[] = {
	{	
		IRP_MJ_WRITE,
		0,
		write_preoperation_callback,
		NULL, //post-operation callback
		NULL},
		

	//more operations to be monitored if needed
	
	{IRP_MJ_OPERATION_END}};
	

const FLT_REGISTRATION Registration__detailes = {
	
	sizeof(FLT_REGISTRATION),
	FLT_REGISTRATION_VERSION,
	NULL,
	context,
	operations, 
	close_filter,//unloading minifilter callback
	NULL, 
	NULL,
	NULL,
	NULL,
	NULL, 
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
	
	
};








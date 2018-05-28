#include <fltkernel.h>
#include "minifilter.h"


//This module simplify the job of registering the minifilter,
//it initialize the objects needed in order to operate the minifilter.



const PFLT_FILTER registration__filter = NULL; 


static const FLT_CONTEXT_REGISTRATION context[] = {
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

static const FLT_OPERATION_REGISTRATION operations[] = {
{	
		IRP_MJ_WRITE,
		0,
		write_preoperation_callback,
		NULL, //post-operation callback
		NULL},
		

	//more operations to be monitored if needed
	
	{IRP_MJ_OPERATION_END}};
	

const FLT_REGISTRATION registration__detailes = {
	
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








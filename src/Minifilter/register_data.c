#include <fltkernel.h>
#include "minifilter.h"


//This module simplify the job of registering the minifilter,
//it initialize the objects needed in order to operate the minifilter.



const PFLT_FILTER registration__filter = NULL; 


static const FLT_CONTEXT_REGISTRATION context[] = {
	{ 
		//the context define what the minifilter will be used for
		FLT_TRANSACTION_CONTEXT, //context type
		0, //flags
		NULL,  //handler to cleanup the contex
		sizeof(FLT_CONTEXT_REGISTRATION) //size
		'damn' //my kickass tag 
		NULL, //context_allocator
		NULL, //context_cleaner
		NULL}, // reserved
		
		
	{FLT_CONTEXT_END} //end of contexts
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
	
	sizeof(FLT_REGISTRATION), //size
	FLT_REGISTRATION_VERSION, //version
	NULL, //flags
	context, 
	operations, //operations used to filter the requests
	minifilter__close_filter,//unloading minifilter callback
	NULL, //called on the first operation of a new volume 
	NULL, //called whenever a request for detaching a instance from a volume
	NULL, //when the teardown starts, the function is needed to compelete all peneded IO
	NULL, //when teardown finished used in order to finish cleanup
	NULL, //something about file name of other minifilters
	NULL,
	NULL,
	NULL,
	NULL,
	NULL	
};








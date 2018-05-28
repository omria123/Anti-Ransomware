#include <fltkernel.h>




// stating the minimum system os version

#define ATLEAST_WIN8     (NTDDI_VERSION >= NTDDI_WIN8)
#define ATLEAST_WIN7     (NTDDI_VERSION >= NTDDI_WIN7)
#define ATLEAST_VISTA    (NTDDI_VERSION >= NTDDI_VISTA)
#define ATLEAST_NOT_W2K  (OSVER(NTDDI_VERSION) > NTDDI_WIN2K)



extern const PFLT_FILTER registration__filter;
extern const FLT_REGISTRATION registration__detailes;


typedef struct _OPLIST {
	FLT_CALLBACK_DATA data;
	OPLIST *next;
} OPLIST;


//minifilter aid operations: 
NTSTATUS close_filter(FLT_FILTER_UNLOAD_FLAGS); //cleaning the Minifilter


//filter operations callbacks:
FLT_PREOP_CALLBACK_STATUS write_preoperation_callback(
	_Inout_ PFLT_CALLBACK_DATA Data;
	_In_ PCFLT_RELATED_OBJECTS FltObjects,
    _Flt_CompletionContext_Outptr_ PVOID *CompletionContext); //the function is set as handler whenever write_IRP is sent




int minifilter__report_operation(PFLT_CALLBACK_DATA Data, PCFLT_RELATED_OBJECTS FltObjects, char* response);

void minfilter__parse_response(response, &response_code, &alternative_data);

void minifilter__finish_operation(NTSTATUS ret_status, );

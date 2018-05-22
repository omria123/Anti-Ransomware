



// stating the minimum system os version

#define ATLEAST_WIN8     (NTDDI_VERSION >= NTDDI_WIN8)
#define ATLEAST_WIN7     (NTDDI_VERSION >= NTDDI_WIN7)
#define ATLEAST_VISTA    (NTDDI_VERSION >= NTDDI_VISTA)
#define ATLEAST_NOT_W2K  (OSVER(NTDDI_VERSION) > NTDDI_WIN2K)



extern const PFLT_FILTER Registaraoion__filter;
extern const FLT_REGISTRATION Registration__detailes;
extern PFLT_PORT  Communication__server_port;


//minifilter aid operations: 
NTSTATUS close_filter(NTSTATUS status, PFLT_FILTER filter); //called after unloading the driver when an error occured




//filter operations callbacks:
FLT_PREOP_CALLBACK_STATUS write_preoperation_callback(
	_Inout_ PFLT_CALLBACK_DATA Data;
	_In_ PCFLT_RELATED_OBJECTS FltObjects,
    _Flt_CompletionContext_Outptr_ PVOID *CompletionContext); //the function is set as handler whenever write_IRP is sent
	
	
	
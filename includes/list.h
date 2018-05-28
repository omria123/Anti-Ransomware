#include <fltkernel.h>


typedef struct _OPLIST {
	PFLT_CALLBACK_DATA data;
	OPLIST next;
	unsigned long oid;
} OPLIST; 


OPLIST* list__find(OPLIST* first, unsigned long id);
void list__add(OPLIST *first, PFLT_CALLBACK_DATA to_add, unsigned long id);
int list__remove(OPLIST*first, unsigned long id);
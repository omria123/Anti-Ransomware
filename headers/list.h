#ifndef LIST__H
#define LIST__H

#include <fltkernel.h>


typedef struct _OPLIST {
	PFLT_CALLBACK_DATA data;
	struct _OPLIST * next;
	unsigned long oid;
} OPLIST; 


OPLIST* list__find(OPLIST* first, unsigned long id);
OPLIST* list__add(OPLIST *first, PFLT_CALLBACK_DATA to_add, unsigned long id);
OPLIST*  list__remove(OPLIST*first, unsigned long id);

#endif


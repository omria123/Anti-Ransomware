#include "list.h"


void list__add(OPLIST *first, PFLT_CALLBACK_DATA to_add, unsigned long id)
{
	if(*first == NULL)
	{
		*first = malloc(sizeof(OPLIST));
		first->data = to_add;
		first->oid = id;
		first->next = NULL;
		return;
	}
	if (first->next==NULL)
	{
		first->next = malloc(sizeof(OPLIST));
		first->data = to_add;
		first->oid = id;
		first->next = NULL;
		return;
	}
	list__add(first->next, to_add, id);


}


OPLIST* list__find(OPLIST* first, unsigned long id)
{
	//find the node of given id return the node or NULL if not found 
	if(first == NULL)
		return NULL;
	if (first->oid == id)
		return first;
	return list__find(first->next, id);
}


OPLIST* list__remove(OPLIST*first, unsigned long id)
{
	//delete if exist first appearance of node with given id
	//returns the new list first node
	OPLIST *tmp;
	if(first == NULL)//if list is empty
		return NULL;

	tmp = first->next;

	if(first->oid ==id)
	{ // check if first node needs to be removed
		if(tmp == NULL)
		{
			free(first);
			return NULL;
		}
	}
	
	if(tmp!= NULL)
	{
		if(tmp->oid == id)
		{//remove next node and return 
			first->next = tmp->next;
			free(tmp);
			return first;
		}
		// call for next node
		list__remove(next, id);
	}

	return first; //end of list and no one found

}

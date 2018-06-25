
#include <wdm.h>
#include "kalloc.h"

static ULONG tag1 = 'myka' , tag2 = 'akym';
static char is1 = TRUE;
static void* kalloc_pool;
static int pages = 0;



char kalloc__check_if_enough_space(char *start_page, int start_alloc, ULONG size)
{
	unsigned int i;
	ULONG left = KALLOC_PAGESIZE - start_alloc;
	char * start = start_page + start_alloc;
	for (i = 0; i<left;i++)
	{
		
		if (*(start++) == '1')
			return 0;
		if (i + 1 == size)
			return 1;
	}
	return 0;
}


void * kalloc__search_page(void *page_start, ULONG size)
{
	char *start = page_start;
	ULONG jump, i;
	for (i=0; i < KALLOC_PAGESIZE; i++)
	{
		if (*(start+i) == '1')
		{
			memcpy(&jump, start + i + 1, ULONG_SIZE);
			i += jump + ULONG_SIZE;
		}
		else 
		{
			if (kalloc__check_if_enough_space(start, i, size+1+ULONG_SIZE))
			{
				return start + i;
			}
			else
			{
				return 0;
			}
		}

	}
	return 0;

}


void kalloc__init()
{
	kalloc_pool = ExAllocatePoolWithTag(PagedPool, (++pages)*KALLOC_PAGESIZE, tag1);
	kalloc__reset_last_page();
}


void * kalloc__malloc(ULONG size)
{

	int i;
	char *pos;
	if (size > KALLOC_PAGESIZE)
		return 0;
	for (i = 0; i < pages; i++)
	{
		pos = kalloc__search_page((char*) kalloc_pool + (pages * KALLOC_PAGESIZE), size);
		if (pos)
		{
			*(pos) = '1';
			memcpy(pos + 1, &size, ULONG_SIZE);
			return (pos + 1 + ULONG_SIZE);
		}

	}
	if (kalloc__more_pages())
	{
		pos = (char*)kalloc_pool + (pages - 1) *KALLOC_PAGESIZE;
		*pos = '1';
		memcpy(pos + 1, &size, ULONG_SIZE);
		return pos + ULONG_SIZE + 1;
	}
	else
		return 0;

}


int kalloc__more_pages()
{
	ULONG tofree = tag2, tomalloc = tag1;
	char *new_pool;
	if (is1)
	{
		tofree = tag1;
		tomalloc = tag2;
	}
	new_pool = ExAllocatePoolWithTag(PagedPool, (++pages)*KALLOC_PAGESIZE, tomalloc);
	if (new_pool == NULL)
		return 0;
	memcpy(new_pool, kalloc_pool, (pages-1)* KALLOC_PAGESIZE);
	ExFreePoolWithTag(kalloc_pool, tofree);
	kalloc_pool = new_pool;
	return 1;  
}

void kalloc__reset_last_page()
{
	char *start = (char*)kalloc_pool + KALLOC_PAGESIZE * (pages - 1);
	int i;
	for (i = 0; i < KALLOC_PAGESIZE; i++)
	{
		*(start++) = 0;
	}

}



void kalloc__free(void *p)
{
	ULONG size;
	char *pos, *start =(char*) p - ULONG_SIZE, *end;

	memcpy(&size, (char*)p - ULONG_SIZE, ULONG_SIZE);
	end = start + ULONG_SIZE + size;

	*((char*)p - ULONG_SIZE - 1) = 0;

	for (pos = start; pos < end; pos++)
	{
		*pos = 0;
	}


}


void kalloc__reset_last_page(void);


void kalloc__free(void *p);
void * kalloc__malloc(ULONG size);

int kalloc__more_pages(void);
void kalloc__init(void);

void * kalloc__search_page(void *page_start, ULONG size);

char kalloc__check_if_enough_space(char *start_page, int start_alloc, ULONG size);

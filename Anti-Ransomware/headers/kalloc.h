#ifndef KALLOC__H
#define KALLOC__H 1

#include <wdm.h>



#define KALLOC_PAGESIZE 4096
#define ULONG_SIZE 4

void kalloc__reset_last_page(void);


void kalloc__free(void *p);
void * kalloc__malloc(ULONG size);

int kalloc__more_pages(void);
void kalloc__init(void);

void * kalloc__search_page(void *page_start, ULONG size);

char kalloc__check_if_enough_space(char *start_page, int start_alloc, ULONG size);

#endif // KALLOC__H

#include <paging.h>

void page_directory(void);
void page_table_0(void);
void page_table_768(void);


#ifndef PAGING_DEFS
#define PAGING_DEFS

#define BYTES_PER_PAGE      4096
#define ENTRIES_PER_TABLE   1024
#define NUM_PAGE_TABLES     1024
#define OS_OFFSET           0xC0000000

uint32_t* pdt;
uint32_t* p_tables[NUM_PAGE_TABLES];


void reinitialize_paging()
{
	// check if this is working correctly
	pdt = (uint32_t *)&page_directory;
	p_tables[0] = (uint32_t *)&page_table_0;
	p_tables[768] = (uint32_t *)&page_table_768;
}

uint32_t *get_physical_address(void *virtual_addr)
{
	uint32_t pd_index = (uint32_t)virtual_addr >> 22;
	uint32_t pt_index = ((uint32_t)virtual_addr >> 12) & 0x3FF;

	uint32_t pd_entry = pdt[pd_index];
	if (!(pd_entry & 0x1))
	{
		printf("Address %u cannot be translated, aborting...\n", (uint32_t)virtual_addr);
		DisableInterrupts();
		for (;;);
	}

	uint32_t *pt_addr = (uint32_t *)(pd_entry & ~0xFFF);
	pt_addr = (uint32_t *)((uint8_t *)pt_addr + OS_OFFSET);

	uint32_t pt_entry = pt_addr[pt_index];
	if (!(pt_entry & 0x1))
	{
		printf("Address %u cannot be translated, aborting...\n", (uint32_t)virtual_addr);
		DisableInterrupts();
		for (;;);
			
	}

	return (uint32_t *)((pt_entry & ~0xFFF) + ((uint32_t)virtual_addr & 0xFFF));
}

#endif
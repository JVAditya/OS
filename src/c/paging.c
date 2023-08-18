#include <paging.h>
#include <stdio.h>
#include <bitmap_alloc.h>

void page_directory(void);
void page_table_0(void);	// all entries in this table are invalidated, this was previously used for identity mapping the OS+bootloader
void page_table_768(void);	// this contains page frames for OS

extern uint32_t __text_start_virt;
extern uint32_t __text_end_virt;
extern uint32_t __boot_start_virt;
extern uint32_t __boot_end_virt;
extern uint32_t __entry_start_virt;
extern uint32_t __entry_end_virt;

extern uint32_t os_end_phy;
extern uint32_t os_start_phy;

#ifndef PAGING_DEFS
#define PAGING_DEFS

#define BYTES_PER_PAGE      4096
#define ENTRIES_PER_TABLE   1024
#define NUM_PAGE_TABLES     1024
#define OS_OFFSET           0xC0000000	// change this if you decide to change initial virtual address mapping
#define NUM_BLOCKS			1 << 20		// assuming 4 GB RAM with 4 KB page blocks
#define FRAMEBUFF_RAM_SIZE	0x800000	// 8 MB is allocated in the RAM for graphics framebuffer

uint32_t* pdt;
uint32_t* p_tables[NUM_PAGE_TABLES];


void make_read_only(uint32_t start_addr, uint32_t end_addr){
	// both start_addr and end_addr are inclusive
	// this function is only for 4KB page page tables
	uint32_t iter_addr = start_addr;
	while(iter_addr <= end_addr){
		uint32_t pd_index = (uint32_t)iter_addr >> 22;		// page directory index
		uint32_t pt_index = ((uint32_t)iter_addr >> 12) & 0x3FF;	// page table index
		uint32_t pd_entry = pdt[pd_index];	// page directory entry

		if (!(pd_entry & 0x1))
		{
			printf("Address %u cannot be translated, aborting...\n", (uint32_t)iter_addr);
			DisableInterrupts();
			for (;;);
		}

		uint32_t *pt_addr = (uint32_t *)(pd_entry & ~0xFFF);	// page table address(physical)
		pt_addr = (uint32_t *)((uint8_t *)pt_addr + OS_OFFSET);	// convert to virtual address (OS offset is 0xC0000000)
		uint32_t pt_entry = pt_addr[pt_index];	// page table entry

		if (!(pt_entry & 0x1))
		{
			printf("Address %u cannot be translated, aborting...\n", (uint32_t)iter_addr);
			DisableInterrupts();
			for (;;);
				
		}
		pt_addr[pt_index] = pt_entry & ~0x02;	// make the page frame Read-Only

		iter_addr += 0x1000;
	}
}

void reinitialize_paging(uint32_t framebuff)
{
	pdt = (uint32_t *)&page_directory;
	p_tables[0] = (uint32_t *)&page_table_0;	// this page table is empty right now
	p_tables[768] = (uint32_t *)&page_table_768;

	printf("page dir: %p\n", &page_directory);
	printf("page table 0: %p\n", &page_table_0);
	printf("page table 768: %p\n", &page_table_768);

	// make the text, boot and entry sections read-only
	make_read_only((uint32_t)&__text_start_virt, (uint32_t)&__text_end_virt);
	make_read_only((uint32_t)&__boot_start_virt, (uint32_t)&__boot_end_virt);
	make_read_only((uint32_t)&__entry_start_virt, (uint32_t)&__entry_end_virt);

	// initialize bitmap
	initialize_bitmap();

	// mark the OS + boot portion (i.e. from 0 to os_end_phy) and 8 MB of graphics as 'used' in the bitmap
	// blocks are 4096 bytes long
	uint32_t start_block_os_boot = 0;	// all inclluding this block have to be set to 1
	uint32_t end_block_os_boot = ((uint32_t)&os_end_phy) >> 12; 	// all including this block have to be set to 1

	set_bitmap_blocks(start_block_os_boot, end_block_os_boot, true);
	
	uint32_t start_block_graphics = framebuff >> 12;
	uint32_t end_block_graphics = start_block_graphics + 2047; 	// all including this block have to be set to 1

	set_bitmap_blocks(start_block_graphics, end_block_graphics, true);

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

	if(pd_entry & (1 << 7)){
		// this corresponds to a 4 MB page
		return (uint32_t*)((pd_entry & ~((1 << 22) - 1)) + ((uint32_t)virtual_addr & ((1 << 22) - 1)));
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
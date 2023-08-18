#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

/*
    This function makes the page directory and the initial page tables available to the C code,
    sets the page table entries corresponding to text, boot and entry sections to READ_ONLY,
    and initializes the bitmap allocator marking the already used portions of memory

    @param framebuff : 32-bit physical address of the graphics framebuffer returned in the multiboot info table
*/
void reinitialize_paging(uint32_t framebuff);

/*
    This function returns the physical corresponding to the virtual address by looking at the page tables. 
    Disables interrupts and halts the OS when a physical address mapping is not found/invalid

    @param virtual _addr : The 32-bit virtual address to be translated
    @returns : 32-bit physical address corresponding to the virtual address
*/
uint32_t* get_physical_address(void* virtual_addr);
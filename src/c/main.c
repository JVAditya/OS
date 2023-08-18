#include <stdint.h>
#include <stdio.h>
#include <memory.h>
#include <loadidt.h>
#include <loadgdt.h>
#include <loadisr.h>
#include <irq.h>
#include <loadisr.h>
#include <keyboard_handler.h>
#include <paging.h>
#include <arch/testing.h>
#include <arch/dis_paging.h>

extern uint32_t os_end_phy;
extern uint32_t os_start_phy;

void timer(Registers* regs){
    return;
}

void __attribute__((section(".text"), cdecl)) start(uint8_t* ebx, uint32_t eax){

    /*
        Entry point into C code from kernel assembly code.
        ebx contains the address to multiboot information structure
        eax contains the multiboot verification value
    */
    DisableInterrupts();  //  is this line correct?

    initialize_gdt();
    initialize_idt();
    initialize_isr();
    initialize_irq();

    EnableInterrupts();     // is this line correct?

    clrscr();

    irq_register_handler(0, timer);
    irq_register_handler(1, keyboard_handler);

    if(eax != 0x2BADB002){
      printf("OS loading failed");
      for(;;);
    }

    uint32_t framebuff = *(uint64_t*)(ebx + 88);

    // graphics framebuffer initialization
    initialize_framebuffer(ebx + 88);

    // font initialization
    initialize_psf();

    printf("\n");
    printf("address of multiboot info table: %p\n", (uint32_t)ebx);
    printf("flags %b\n", *(uint32_t*)(ebx));
    printf("mem_low: %p kilobytes\n", *(uint32_t*)(ebx + 4));
    printf("mem_high: %p kilobytes\n", *(uint32_t*)(ebx + 8));
    printf("bootloader: %s\n", *(uint32_t*)(ebx + 64));

    printf("Frame buffer: %p\n", *(uint64_t*)(ebx + 88));
    printf("Frame buffer pitch: %u\n", *(uint32_t*)(ebx + 96));
    printf("Frame buffer width: %u\n", *(uint32_t*)(ebx + 100));
    printf("Frame buffer height: %u\n", *(uint32_t*)(ebx + 104));
    printf("Frame buffer depth: %u\n", *(uint8_t*)(ebx + 108));
    printf("\n\t\tKernel starts at phy. address %p and ends at phy. address %p \n\n", (uint32_t)(&os_start_phy), (uint32_t)(&os_end_phy));    

    // disable identity paging now
    disable_identity_paging();

    // let the C code know of the locations of page tables and page directory
    reinitialize_paging(framebuff);
    
    for(;;);

}
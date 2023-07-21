#include <stdint.h>
#include <stdio.h>
#include <memory.h>
#include <loadidt.h>
#include <loadgdt.h>
#include <loadisr.h>
#include <irq.h>
#include <keyboard_handler.h>

void crash_me();

void timer(Registers* regs){
    return;
}

void __attribute__((section(".entry"))) start(){

    /*
        Entry point into C code from kernel assembly code
    */

    initialize_gdt();
    initialize_idt();
    initialize_isr();
    initialize_irq();
    clrscr();
    puts("LMAO\n\0");
    // crash_me();
    
    irq_register_handler(0, timer);
    irq_register_handler(1, keyboard_handler);

    for(;;);

}
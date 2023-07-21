#include <irq.h>
#include <pic.h>
#include <stdio.h>
#include <arch/io.h>
#include <stddef.h>
#include <intr_num.h>


#define PIC_REMAP_OFFSET    0x20

IRQHandler g_IRQHandlers[16];

void irq_handler(Registers* regs){
    int irq = regs->interrupt - PIC_REMAP_OFFSET;
    if(g_IRQHandlers[irq] != NULL){
        g_IRQHandlers[irq](regs);
    }
    else{
        puts("Unhandled IRQ \0");
        put_intr(irq);
        putc('\n');
    }

    PIC_SendEndOfInterrupt(irq);
}

void initialize_irq(){
    PIC_Configure(PIC_REMAP_OFFSET, PIC_REMAP_OFFSET + 8);

    for(int i = 0; i < 16; i++){
        isr_register_handler(PIC_REMAP_OFFSET + i, irq_handler);
    }

    EnableInterrupts();
}

void irq_register_handler(int irq, IRQHandler handler){
    g_IRQHandlers[irq] = handler;
}
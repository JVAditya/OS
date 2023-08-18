#include <irq.h>
#include <pic.h>
#include <stdio.h>
#include <arch/io.h>
#include <stddef.h>


#define PIC_REMAP_OFFSET    0x20    // 0x20 = 32 is the ID of the first 'interrupt' (not exception) in the IDT

IRQHandler g_IRQHandlers[16];   // store handlers for the 16 IRQs 

void irq_handler(Registers* regs){
    int irq = regs->interrupt - PIC_REMAP_OFFSET;   // IRQ index = interrupt_number - 32
    if(g_IRQHandlers[irq] != NULL){
        g_IRQHandlers[irq](regs);
    }
    else{
        printf("Unhandled IRQ %d \n", irq);
    }

    PIC_SendEndOfInterrupt(irq);
}

void initialize_irq(){
    PIC_Configure(PIC_REMAP_OFFSET, PIC_REMAP_OFFSET + 8);

    for(int i = 0; i < 16; i++){
        isr_register_handler(PIC_REMAP_OFFSET + i, irq_handler);    // use IRQ handler for interrupt IDs 32-47
    }

    EnableInterrupts();
}

void irq_register_handler(int irq, IRQHandler handler){
    g_IRQHandlers[irq] = handler;
}
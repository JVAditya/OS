#include <pic.h>
#include <arch/io.h>

void PIC_Configure(uint8_t offsetPIC1, uint8_t offsetPIC2){
    outb(PIC1_COMMAND_PORT, PIC_ICW1_ICW4 | PIC_ICW1_INITIALIZE);
    iowait();
    outb(PIC2_COMMAND_PORT, PIC_ICW1_ICW4 | PIC_ICW1_INITIALIZE);
    iowait();

    outb(PIC1_DATA_PORT, offsetPIC1);
    iowait();
    outb(PIC2_DATA_PORT, offsetPIC2);
    iowait();

    outb(PIC1_DATA_PORT, 0x4);
    iowait();
    outb(PIC2_DATA_PORT, 0x2);
    iowait();

    outb(PIC1_DATA_PORT, PIC_ICW4_8086);
    iowait();
    outb(PIC2_DATA_PORT, PIC_ICW4_8086);
    iowait();

    outb(PIC1_DATA_PORT, 0);
    iowait();
    outb(PIC2_DATA_PORT, 0);
    iowait();
}

void PIC_Mask(int irq){
    if(irq < 8){
        uint8_t mask = inb(PIC1_DATA_PORT);
        outb(PIC1_DATA_PORT, mask | (1 << irq));
    }
    else{
        irq -= 8;
        uint8_t mask = inb(PIC2_DATA_PORT);
        outb(PIC2_DATA_PORT, mask | (1 << irq));
    }
}

void PIC_Unmask(int irq){
    if(irq < 8){
        uint8_t mask = inb(PIC1_DATA_PORT);
        outb(PIC1_DATA_PORT, mask & ~(1 << irq));
    }
    else{
        irq -= 8;
        uint8_t mask = inb(PIC2_DATA_PORT);
        outb(PIC2_DATA_PORT, mask & ~(1 << irq));
    }
}

void PIC_Disable(){
    outb(PIC1_DATA_PORT, 0xFF);
    iowait();
    outb(PIC2_DATA_PORT, 0xFF);
    iowait();
}

void PIC_SendEndOfInterrupt(int irq){
    if(irq >= 8){
        outb(PIC2_COMMAND_PORT, PIC_CMD_END_OF_INTERRUPT);
    }
    outb(PIC1_COMMAND_PORT, PIC_CMD_END_OF_INTERRUPT);
}

uint16_t PIC_ReadIRQRequestRegister(){
    outb(PIC1_COMMAND_PORT, PIC_CMD_READ_IRR);
    outb(PIC2_COMMAND_PORT, PIC_CMD_READ_IRR);
    return ((uint16_t)inb(PIC2_COMMAND_PORT) | ((uint16_t)inb(PIC2_COMMAND_PORT) << 8));
}

uint16_t PIC_ReadIRQServiceRegister(){
    outb(PIC1_COMMAND_PORT, PIC_CMD_READ_ISR);
    outb(PIC2_COMMAND_PORT, PIC_CMD_READ_ISR);
    return ((uint16_t)inb(PIC2_COMMAND_PORT) | ((uint16_t)inb(PIC2_COMMAND_PORT) << 8));
}
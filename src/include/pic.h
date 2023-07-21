#pragma once

#include <arch/pic_util.h>
#include <stdint.h>

void PIC_Configure(uint8_t offsetPIC1, uint8_t offsetPIC2);
void PIC_Mask(int irq);
void PIC_Unmask(int irq);
void PIC_Disable();
void PIC_SendEndOfInterrupt(int irq);
uint16_t PIC_ReadIRQRequestRegister();
uint16_t PIC_ReadIRQServiceRegister();
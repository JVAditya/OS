#pragma once
#include <loadisr.h>

typedef void (*IRQHandler)(Registers* regs);

void initialize_irq();
void irq_register_handler(int irq, IRQHandler handler);
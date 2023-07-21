#pragma once
#include <arch/idt.h>

void initialize_idt();
void idt_DisableGate(int interrupt);
void idt_EnableGate(int interrupt);
void idt_SetGate(int interrupt, void* base, uint16_t segmentDescriptor, uint8_t flags);

// void interrupt_handler(cpu_state cpu, stack_state stack, uint32_t interrupt);
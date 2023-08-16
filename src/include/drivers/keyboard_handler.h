#pragma once
#include <irq.h>
#include <arch/io.h>

#define KBD_DATA_PORT   0x60

void keyboard_handler(Registers* regs);
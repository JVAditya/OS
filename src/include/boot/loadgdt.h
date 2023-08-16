#pragma once
#include <arch/gdt.h>

/*
    Creates a GDT structure for CPU to use. Only contains cs and data descriptors along with null descriptor
*/
void initialize_gdt();
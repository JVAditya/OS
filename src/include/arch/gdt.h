#pragma once
#include "gdt_util.h"

#define GDT_CODE_SEGMENT 0x08
#define GDT_DATA_SEGMENT 0x10

/*
    Loads the created GDT into GDTR register 
    @param gdtr : GDT summary struct that contains limit value and GDT address
    @param code_segment : Code descriptor byte offset in GDT
    @param data_segment: Data descriptors' byte offset in GDT
*/
void __attribute__((cdecl)) load_gdt(GDTR* gdtr, uint16_t code_segment, uint16_t data_segment);


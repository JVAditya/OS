#pragma once
#include "idt_util.h"

/*
    Loads the created IDT into IDTR register 
    @param idtr : IDT summary struct that contains limit value and IDT address
*/
void __attribute__((cdecl)) load_idt(IDTR* idtr);

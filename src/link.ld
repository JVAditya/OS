ENTRY(entry)

SECTIONS {
    . = 0x00100000;

    .boot ALIGN (0x1000) :
    {
        __boot_start = .;
        *(.boot)
    }

    .entry ALIGN (0x1000) :
    {
        __entry_start = .;
        *(.entry)
    }

    .text ALIGN (0x1000) :    
    {   
        __text_start = .;
        *(.text)
    }

    .data ALIGN (0x1000) :
    {
        __data_start = .;
        *(.data)
    }

    .rodata ALIGN (0x1000) :
    {
        __rodata_start = .;
        *(.rodata*)
    }

    .bss ALIGN (0x1000) :
    {
        __bss_start = .; 
        *(COMMON)
        *(.bss)
    }

    __end = .;
}

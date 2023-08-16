ENTRY(entry)

SECTIONS {
    . = 0x00100000;
    os_start = .;

    .boot ALIGN (4K) :
    {
        __boot_start = .;
        *(.boot)
    }

    .entry ALIGN (4K) :
    {
        __entry_start = .;
        *(.entry)
    }

    . += 0xC0000000;
    .text ALIGN (4K) : AT (ADDR (.text) - 0xC0000000)
    {   
        __text_start = .;
        *(.text)
    }

    .data ALIGN (4K) : AT (ADDR (.data) - 0xC0000000)
    {
        __data_start = .;
        *(.data)
    }

    .rodata ALIGN (4K) : AT (ADDR (.rodata) - 0xC0000000)
    {
        __rodata_start = .;
        *(.rodata*)
    }

    .bss ALIGN (4K) : AT (ADDR (.bss) - 0xC0000000)
    {
        __bss_start = .; 
        *(COMMON)
        *(.bss)
    }

    os_end = .;
}
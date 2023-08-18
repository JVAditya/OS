ENTRY(entry)

SECTIONS {
    . = 0x00100000;
    os_start_phy = .;
    os_start_virt = . + 0xC0000000;

    .boot ALIGN (4K) :
    {
        __boot_start_phy = .;
        __boot_start_virt = . + 0xC0000000;
        *(.boot)
        __boot_end_phy = .;
        __boot_end_virt = . + 0xC0000000;
    }

    .entry ALIGN (4K) :
    {
        __entry_start_phy = .;
        __entry_start_virt = . + 0xC0000000;
        *(.entry)
        __entry_end_phy = .;
        __entry_end_virt = . + 0xC0000000;
    }

    . += 0xC0000000;
    .text ALIGN (4K) : AT (ADDR (.text) - 0xC0000000)
    {   
        __text_start_phy = . - 0xC0000000;
        __text_start_virt = .;
        *(.text)
        __text_end_phy = . - 0xC0000000;
        __text_end_virt = .;
    }

    .data ALIGN (4K) : AT (ADDR (.data) - 0xC0000000)
    {
        __data_start_phy = . - 0xC0000000;
        __data_start_virt = .;
        *(.data)
        __data_end_phy = . - 0xC0000000;
        __data_end_virt = .;
    }

    .rodata ALIGN (4K) : AT (ADDR (.rodata) - 0xC0000000)
    {
        __rodata_start_phy = . - 0xC0000000;
        __rodata_start_virt = .;
        *(.rodata*)
        __rodata_end_phy = . - 0xC0000000;
        __rodata_end_virt = .;
    }

    .bss ALIGN (4K) : AT (ADDR (.bss) - 0xC0000000)
    {
        __bss_start_phy = . - 0xC0000000;
        __bss_start_virt = .; 
        *(COMMON)
        *(.bss)
        __bss_end_phy = . - 0xC0000000;
        __bss_end_virt = .;
    }
    os_end_phy = . - 0xC0000000;
    os_end_virt = .;
}
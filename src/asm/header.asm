MB_ALIGN                        equ 1 << 0  ; multiboot modules aligned to 4096 bytes
MEM_INFO                        equ 1 << 1  ; provide memory info such as mem_low and mem_high
VIDEO                           equ 1 << 2  ; provide a framebuffer to draw graphics

MODE_TYPE                       equ 0       ; graphics mode - we need a framebuffer so set it to 0
WIDTH                           equ 640     ; preferred width
HEIGHT                          equ 480     ; preferred height
DEPTH                           equ 32      ; preferred pixel depth
HEADER_ADDR                     equ 0
LOAD_ADDR                       equ 0
LOAD_END_ADDR                   equ 0
BSS_END_ADDR                    equ 0
ENTRY_ADDR                      equ 0

MAGIC_NUMBER                    equ 0x1BADB002 
FLAGS                           equ MB_ALIGN | MEM_INFO | VIDEO 
CHECKSUM                        equ -(MAGIC_NUMBER + FLAGS)

section .boot
align 4096
    dd MAGIC_NUMBER     ; magic number
    dd FLAGS            ; flags
    dd CHECKSUM         ; checksum
    dd HEADER_ADDR      ; header_addr
    dd LOAD_ADDR        ; load_addr
    dd LOAD_END_ADDR    ; load_end_addr
    dd BSS_END_ADDR     ; bss_end_addr
    dd ENTRY_ADDR       ; entry_addr
    dd MODE_TYPE        ; mode_type
    dd WIDTH            ; width
    dd HEIGHT           ; height
    dd DEPTH            ; depth

global entry

extern start

KERNEL_STACK_SIZE               equ 4096
FRAME_BUFFER                    equ 0x000B8000

section .entry
align 4
    entry:
        mov eax, 0xCAFEBABE
        mov ebp, esp
        mov esp, stack_top
        push ebx
        call start

section .bss
align 4
    resb KERNEL_STACK_SIZE
    stack_top:

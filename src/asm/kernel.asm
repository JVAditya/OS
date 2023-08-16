global entry
global disable_identity_paging
global graphics_page_entry
global page_directory
global page_table_0
global page_table_768
global test_work

extern start

KERNEL_STACK_SIZE               equ 0x100000
PRESENT                         equ 0x1     ; is the page actually in physical memory or has it been swapped out?
READ_WRITE                      equ 0x2     ; if set, then the page is both read/write, else read-only
USER_SUPERVISOR                 equ 0x4     ; if set, then page can be accessed by all
WRITE_THROUGH                   equ 0x8     ; this bit disables write througgh
DISABLE_CACHE                   equ 0x10    ; this bit disables the cache
ACCESSED                        equ 0x20    ; if the bit is set, then the page has been accessed, this bit is not cleared by the CPU
PAGE_SIZE                       equ 0x80    ; setting this makes it a 4 MB page

DEFAULT_FLAGS                   equ PRESENT | READ_WRITE

section .entry
align 4096
    entry:
        ; identity page the first megabyte

        mov edi, page_directory - 0xC0000000
        mov esi, page_table_0 - 0xC0000000

        mov ecx, esi    ; set 32 bits of first PD entry to 0
        or ecx, DEFAULT_FLAGS  ; set flags
        mov [edi], ecx  ; set entry 0 of page descriptor

        ; fill the page table for boot sector

        mov ecx, 0
        mov edi, 0
        .fill_page_boot:
            ; upto 4 MB fits on one Page Table

            mov edx, ecx
            or edx, DEFAULT_FLAGS
            mov [esi + 4*edi], edx
            add ecx, 4096
            inc edi
            cmp edi, 1024
            jl .fill_page_boot

        ; now map the OS to 0xC0100000 (virtual address). Assuming that the kernel fits in 4 MB

        mov edi, page_directory - 0xC0000000
        mov esi, page_table_768 - 0xC0000000

        mov ecx, esi
        or ecx, DEFAULT_FLAGS  ; set flags
        mov [edi + 4*768], ecx

        ; fill the page table for OS

        mov ecx, 0x100000
        mov edi, 256    ; 256*4096 = 0x100000
        mov esi, page_table_768 - 0xC0000000
        .fill_page_os:
            ; upto 4 MB fits on one Page Table

            mov edx, ecx
            or edx, DEFAULT_FLAGS
            mov [esi + 4*edi], edx
            add ecx, 4096
            inc edi
            cmp edi, 1024
            jl .fill_page_os

        ; video buffer
        mov ecx, 0x000B8000
        or ecx, DEFAULT_FLAGS
        mov [page_table_768 - 0xC0000000 + 1023 * 4], ecx

        ; basic paging is over, now enable paging
        mov ecx, page_directory - 0xC0000000
        mov cr3, ecx

        mov ecx, cr4
        or ecx, 0x10
        mov cr4, ecx

        mov ecx, cr0
        or ecx, 0x80000001
        mov cr0, ecx

        jmp start_os

section .text
align 4096
    start_os:
        mov ebp, esp
        mov esp, stack_top
        push eax
        push ebx    ; contains information about multiboot headers

        ; need to disable identity mapping after reading the multiboot information table
        call start

    graphics_page_entry:
        push ebp
        mov ebp, esp

        push eax
        push ebx
        push ecx
        push edx

        mov eax, [ebp + 8]
        mov ebx, [ebp + 12]
        mov ecx, [ebp + 16]
        mov edx, [ebp + 20]

        or eax, 387
        mov [page_directory + 1012*4], eax

        pop edx
        pop ecx
        pop ebx
        pop eax

        mov esp, ebp
        pop ebp
        ret

    disable_identity_paging:
        push ecx

        mov dword [page_directory], 0

        mov ecx, cr3
        mov cr3, ecx

        pop ecx
        ret

    test_work:
        mov ecx, [esp + 4]
        mov ebx, 0xcafebabe
        cli
        hlt
        jmp $
        ret
        

section .bss
align 4096
    resb KERNEL_STACK_SIZE
    stack_top:

    page_directory:
        resb 4096

    page_table_0:
        resb 4096

    page_table_768:
        resb 4096

    





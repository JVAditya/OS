global load_gdt

; load_gdt : Loads the GDT from address in the input
; [esp + 4] : address of GDT struct
; [esp + 8] : code segment byte offset in GDT
; [esp + 12] : data segment byte offset in GDT

load_gdt:
align 4
    push ebp
    mov ebp, esp
    
    mov eax, [ebp + 8]
    lgdt [eax]

    mov eax, [ebp + 12]
    push eax
    push .reload_cs
    retf

    .reload_cs:
        mov ax, [ebp + 16]
        mov ds, ax
        mov ss, ax
        mov es, ax
        mov fs, ax
        mov gs, ax

    mov esp, ebp
    pop ebp
    ret
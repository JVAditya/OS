global load_idt

; load_idt : Loads the IDT from address in the input
; [esp + 4] : address of IDT struct

load_idt:
align 4
    push ebp
    mov ebp, esp
    
    mov eax, [ebp + 8]
    lidt [eax]

    mov esp, ebp
    pop ebp
    ret
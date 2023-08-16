global load_idt


section .text
align 4096
    ; load_idt : Loads the IDT from address in the input
    ; [esp + 4] : address of IDT struct
    load_idt:

        push ebp
        mov ebp, esp

        mov eax, [ebp + 8]
        lidt [eax]

        mov esp, ebp
        pop ebp
        ret
global outb
global inb
global panic
global EnableInterrupts
global DisableInterrupts

section .text
align 4096
    outb:
    ; outb - sends a byte to an I/O port
    ; [esp + 8] data byte
    ; [esp + 4] I/O port
        push ebp
        mov ebp, esp

        mov al, [ebp + 12]   ; data byte in al
        mov dx, [ebp + 8]   ; port number in dx
        out dx, al

        mov esp, ebp
        pop ebp
        ret

    inb:
    ; inb - receives a byte from an I/O port
    ; [esp + 4] I/O port
        push ebp
        mov ebp, esp

        mov dx, [ebp + 8]   ; port number in dx
        xor eax, eax
        in al, dx

        mov esp, ebp
        pop ebp
        ret

    panic:
        cli 
        hlt

    EnableInterrupts:
        sti 
        ret
    
    DisableInterrupts:
        cli
        ret
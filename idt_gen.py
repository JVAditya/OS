errors = [8, 10, 11, 12, 13, 14, 17, 21, 29, 30]

with open('isrgen.inc', 'w') as f:
    for i in range(0, 256):
        f.write(f"idt_SetGate({i}, isr{i}, GDT_CODE_SEGMENT, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT);\n")
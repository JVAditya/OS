#include <loadidt.h>
#include <binary.h>

IDTEntry g_IDT[256];
IDTR g_idtr = {sizeof(g_IDT) - 1, g_IDT};

void idt_SetGate(int interrupt, void* base, uint16_t segmentDescriptor, uint8_t flags)
{
    g_IDT[interrupt].base_low = ((uint32_t)base) & 0xFFFF;
    g_IDT[interrupt].segment_selector = segmentDescriptor;
    g_IDT[interrupt].reserved = 0;
    g_IDT[interrupt].flags = flags;
    g_IDT[interrupt].base_high = ((uint32_t)base >> 16) & 0xFFFF;
}

void idt_EnableGate(int interrupt)
{
    FLAG_SET(g_IDT[interrupt].flags, IDT_FLAG_PRESENT);
}

void idt_DisableGate(int interrupt)
{
    FLAG_UNSET(g_IDT[interrupt].flags, IDT_FLAG_PRESENT);
}

void initialize_idt(){
    load_idt(&g_idtr);
}

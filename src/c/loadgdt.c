#include <loadgdt.h>

#define GDT_BASE_HIGH(base)                     ((base >> 24) & 0xFF)
#define GDT_BASE_MIDDLE(base)                      ((base >> 16) & 0xFF)
#define GDT_BASE_LOW(base)                      (base & 0xFFFF)
#define GDT_LIMIT_LOW(limit)                    (limit & 0xFFFF)
#define GDT_LIMIT_HIGH__FLAGS(limit, flags)     (((limit >> 16) & 0xF) | (flags & 0xF0))

#define GDT_ENTRY(base, limit, access, flags) {                     \
    GDT_LIMIT_LOW(limit),                                           \
    GDT_BASE_LOW(base),                                             \
    GDT_BASE_MIDDLE(base),                                          \
    access,                                                         \
    GDT_LIMIT_HIGH__FLAGS(limit, flags),                            \
    GDT_BASE_HIGH(base)                                             \
}

GDTEntry g_GDT[] = {
    // null descriptor
    GDT_ENTRY(0, 0, 0, 0),
    
    // 32-bit code segment
    GDT_ENTRY(0,
              0xFFFFF,
              (GDT_ACCESS_PRESENT | GDT_ACCESS_RING0 | GDT_ACCESS_CODE_SEGMENT | GDT_ACCESS_CODE_READABLE),
              (GDT_FLAG_32BIT | GDT_FLAG_GRANULARITY_4K)),

    // 32-bit data segment
     GDT_ENTRY(0,
              0xFFFFF,
              (GDT_ACCESS_PRESENT | GDT_ACCESS_RING0 | GDT_ACCESS_DATA_SEGMENT | GDT_ACCESS_DATA_WRITEABLE),
              (GDT_FLAG_32BIT | GDT_FLAG_GRANULARITY_4K))

};

GDTR g_gdtr = {sizeof(g_GDT) - 1, g_GDT};

void initialize_gdt(){
    load_gdt(&g_gdtr, GDT_CODE_SEGMENT, GDT_DATA_SEGMENT);
}
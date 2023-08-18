#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/*
    Initializes bitmap array by making the bitmap available to C code and sets all bytes to 0 (unused)
*/
void initialize_bitmap();

/*
    Sets the bits of the bitmap array corresponding to the region [start_block, end_block] to 1 (used)
*/
void set_bitmap_blocks(uint32_t start_block, uint32_t end_block, bool set);

#include <bitmap_alloc.h>

void bitmap_mem(void);

#ifndef BITMAP_DEFS
#define BITMAP_DEFS

#define BITMAP_BYTES		1 << 17		// assuming 4 GB RAM with 4 KB page blocks

uint8_t* mem_bitmap_arr = (uint8_t*)&bitmap_mem;

void initialize_bitmap(){
    uint8_t* bmp_pass = mem_bitmap_arr;
	for(int i = 0; i < BITMAP_BYTES; i += 4){
		uint32_t* bmp_int = (uint32_t*)bmp_pass;
		*bmp_int = 0;
		bmp_pass += 4;
	}
}

void set_bitmap_blocks(uint32_t start_block, uint32_t end_block, bool set){
	// including end_block and start_block
	uint32_t num_rem_bits_start = start_block % 8;
	if(set){
		mem_bitmap_arr[start_block/8] |= (1 << (8 - num_rem_bits_start)) - 1;
	}
	else{
		mem_bitmap_arr[start_block/8] &= ~((uint8_t)((1 << (8 - num_rem_bits_start)) - 1));
	}
	for(uint32_t i = (start_block/8) + 1; i < (end_block/8); i++){	
		if(set){
			mem_bitmap_arr[i] = 0xff;
		}
		else{
			mem_bitmap_arr[i] = 0x0;
		}
	}
	uint32_t num_rem_bits_end = end_block % 8;
	if(set){
		mem_bitmap_arr[end_block/8] |= ~((uint8_t)((1 << (7 - num_rem_bits_end)) - 1));
	}
	else{
		mem_bitmap_arr[end_block/8] &= ((uint8_t)((1 << (7 - num_rem_bits_end)) - 1));
	}
}



#endif
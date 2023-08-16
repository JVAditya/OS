#include <stdio.h>
#include <arch/io.h>
#include <arch/dis_paging.h>
#include <stdbool.h>

extern uint8_t _binary_utilities_UbuntuMono_R_8x16_psf_start;
extern uint8_t _binary_utilities_UbuntuMono_R_8x16_psf_end;

uint8_t* g_ScreenBuffer = (uint8_t*)(VGA_BUFFER_ADDR);
int g_ScreenX = 0, g_ScreenY = 0;

uint8_t* frame_buffer;
uint32_t frame_buffer_pitch, frame_buffer_width, frame_buffer_height, frame_buffer_bpp;
const uint32_t char_width = 8;	// in pixels
const uint32_t char_height = 16;	// in pixels

// const uint32_t SCREEN_WIDTH = 80;
// const uint32_t SCREEN_HEIGHT = 25;
// const uint8_t DEFAULT_COLOR = 0x7;

uint32_t SCREEN_WIDTH;
uint32_t SCREEN_HEIGHT;
uint32_t DEFAULT_COLOR = 0x0;

PSF1_HEADER* font_header;
uint8_t* font_data;

void initialize_framebuffer(uint8_t* ebx){
	FrameBufferInfo* fb = (FrameBufferInfo*)(ebx + 88);
	frame_buffer = (uint8_t*)(fb->framebuffer_addr_low);
	frame_buffer_width = fb->framebuffer_width;
	frame_buffer_height = fb->framebuffer_height;
	frame_buffer_pitch = fb->framebuffer_pitch;
	frame_buffer_bpp = fb->framebuffer_bpp;

	SCREEN_WIDTH = frame_buffer_width/char_width;
	SCREEN_HEIGHT = frame_buffer_height/char_height;

	// now identity map the frame_buffer address
	graphics_page_entry(fb->framebuffer_addr_low, frame_buffer_width, frame_buffer_height, frame_buffer_bpp);
}

// load font function
void initialize_psf(){	
	font_header = (PSF1_HEADER*)(&_binary_utilities_UbuntuMono_R_8x16_psf_start); 
	font_data = (uint8_t*)(font_header) + 3;  // 3 is the size of header in PSF1
}

void clear_cell_gfx(uint32_t x, uint32_t y, uint32_t color){
	for(uint32_t r = 0; r < 16; r++){
		for(uint32_t c = 0; c < 8; c++){
			uint32_t index_ = (y*char_height + r)*(frame_buffer_pitch) + (x*char_width + c)*4;
			frame_buffer[index_] = color & 0xff;
			frame_buffer[index_ + 1] = (color >> 8) & 0xff;
			frame_buffer[index_ + 2] = (color >> 16) & 0xff;
		}
	} 
}

void put_chr(int x, int y, uint8_t ch){
    /*
        Places character ch at (x, y) on framebuffer. Each position has 2 bytes - character, color
        @param x: x offset of character
        @param y: y offset of character
        @param ch: character to be written
    */
    // g_ScreenBuffer[2 * (x + y * SCREEN_WIDTH)] = ch;

	uint8_t* glyph = font_data + ch*16;  // glyphs start after 3 bytes of header
	uint32_t white = 0xffffff;
	for(uint32_t r = 0; r < 16; r++){
		uint8_t cols = 1 << 7;
		for(uint32_t c = 0; c < 8; c++){
			if(*glyph & cols){
				uint32_t index_ = (y*char_height + r)*(frame_buffer_pitch) + (x*char_width + c)*4;
				frame_buffer[index_] = white & 0xff;
				frame_buffer[index_ + 1] = (white >> 8) & 0xff;
				frame_buffer[index_ + 2] = (white >> 16) & 0xff;
			}
			cols = cols >> 1;
		} 
		glyph++;
	}
}

void put_color(int x, int y, uint8_t color){
    /*
        Foreground color is applied at (x, y) on framebuffer
        @param x: x offset of character
        @param y: y offset of character
        @param color: foreground color of character | background color of character
    */
    g_ScreenBuffer[2 * (x + y * SCREEN_WIDTH) + 1] = color;
}

uint8_t get_chr(int x, int y){
    /*
        Returns character placed at position (x, y) on framebuffer
        @param x: x offset of character
        @param y: y offset of character
        @returns: character at position (x, y)
    */
    return g_ScreenBuffer[2 * (x + y * SCREEN_WIDTH)];
}

uint8_t get_color(int x, int y){
    /*
        Returns color of character placed at position (x, y) on framebuffer
        @param x: x offset of character
        @param y: y offset of character
        @returns: color of character at position (x, y)
    */
    return g_ScreenBuffer[2 * (x + y * SCREEN_WIDTH) + 1];
}

void set_cursor(int x, int y){
    /*
        Moves the cursor to position (x, y)
        @param x: x offset of cursor
        @param y: y offset of cursor
    */
    // int pos = x + y * SCREEN_WIDTH;
    // outb(FB_STATUS_PORT, 0x0F);     // tell the port that lower 8 bits are being sent
    // outb(FB_DATA_PORT, (uint8_t)(pos & 0xFF));
    // outb(FB_STATUS_PORT, 0x0E);     // tell the port that higher 8 bits are being sent
    // outb(FB_DATA_PORT, (uint8_t)((pos >> 8) & 0xFF));

    // g_ScreenX = x;
    // g_ScreenY = y;

	g_ScreenX = x;
	g_ScreenY = y;
}

void clrscr(){
    
    // for(uint32_t y = 0; y < SCREEN_HEIGHT; y++){
    //     for(uint32_t x = 0; x < SCREEN_WIDTH; x++){
    //         put_chr(x, y, '\0');
    //         put_color(x, y, DEFAULT_COLOR);
    //     }
    // }

    // g_ScreenX = 0;
    // g_ScreenY = 0;
    // set_cursor(g_ScreenX, g_ScreenY);
	for(uint32_t y = 0; y < SCREEN_HEIGHT; y++){
        for(uint32_t x = 0; x < SCREEN_WIDTH; x++){
            clear_cell_gfx(x, y, DEFAULT_COLOR);
        }
    }
	g_ScreenX = 0;
    g_ScreenY = 0;
    set_cursor(g_ScreenX, g_ScreenY);


}

void scrolldown(uint32_t num_lines){
    /* 
        scroll down is positive, scroll up is negative
        @param num_lines: Number of lines to scroll down
    */
    // for(uint32_t y = num_lines; y < SCREEN_HEIGHT; y++){
    //     for(uint32_t x = 0; x < SCREEN_WIDTH; x++){
    //         uint8_t chr = get_chr(x, y);
    //         put_chr(x, y, '\0');
            
    //         uint8_t color = get_color(x, y);
    //         put_color(x, y, DEFAULT_COLOR);

    //         put_chr(x, y - num_lines, chr);
    //         put_color(x, y - num_lines, color);
    //     }
    // }
    // g_ScreenY -= num_lines;
    // if(g_ScreenY < 0){
    //     g_ScreenY = 0;
    // }
    // set_cursor(g_ScreenX, g_ScreenY);
	
	for(uint32_t y = num_lines*char_height; y < SCREEN_HEIGHT*char_height; y++){
	    for(uint32_t x = 0; x < SCREEN_WIDTH*char_width; x++){
			uint32_t index__ = (y - num_lines*char_height)*(frame_buffer_pitch) + (x)*4;
			uint32_t index_ = (y)*(frame_buffer_pitch) + (x)*4;
			frame_buffer[index__] = frame_buffer[index_];
			frame_buffer[index__ + 1] = frame_buffer[index_ + 1];
			frame_buffer[index__ + 2] = frame_buffer[index_ + 2];
			frame_buffer[index_] = 0;
			frame_buffer[index_ + 1] = 0;
			frame_buffer[index_ + 2] = 0;
	    }
	}
	g_ScreenY -= num_lines;
	if(g_ScreenY < 0){
	    g_ScreenY = 0;
	}
	set_cursor(g_ScreenX, g_ScreenY);
}

void putc(char c){
    
    if(c == '\n'){
        g_ScreenX = 0;
        g_ScreenY++;
        if(g_ScreenY == (int)SCREEN_HEIGHT){
            scrolldown(1);
            g_ScreenY = SCREEN_HEIGHT - 1;
        }
    }
    else if(c == '\r'){
        g_ScreenX = 0;
    }
    else if(c == '\t'){
        for (int i = 0; i < 4 - (g_ScreenX % 4); i++){
            putc(' ');
        }
    }
    else{
        put_chr(g_ScreenX, g_ScreenY, c);
        if(g_ScreenX < (int)SCREEN_WIDTH-1){
            g_ScreenX++;
        }
        else{
            g_ScreenX = 0;
            if(g_ScreenY < (int)SCREEN_HEIGHT - 1){
                g_ScreenY++;
            }
            else{
                scrolldown(1);
                g_ScreenY = (int)SCREEN_HEIGHT - 1;
                return;
            }
        }
    }

    set_cursor(g_ScreenX, g_ScreenY);
}

void puts(const char* str){
    uint8_t* ptr = (uint8_t*)str;
    while(*ptr){
        putc(*ptr);
        ptr++;
    }
}

int __attribute__((cdecl)) printf(const char *fmt, ...)
{
	int32_t *args = (int32_t *)(&fmt);
	args++;

	uint32_t state = PRINTF_NORMAL_STATE;
	uint32_t len_state = PRINTF_NORMAL_INT;

	while (*fmt)
	{
		uint8_t ch = *fmt;
		if (state == PRINTF_NORMAL_STATE)
		{
			if (ch == '%')
			{
				state = PRINTF_FORMAT_STATE;
			}
			else
			{
				putc(ch);
			}
		}

		else if (state == PRINTF_FORMAT_STATE)
		{
			if (ch == '%')
			{
				// print % symbol
				putc(ch);
				state = PRINTF_NORMAL_STATE;
			}
			else if (ch == 'h')
			{
				if (len_state == PRINTF_NORMAL_INT)
				{
					len_state = PRINTF_SHORT_INT;
				}
				else if (len_state == PRINTF_SHORT_INT)
				{
					len_state = PRINTF_CHAR_INT;
				}
				else
				{
					return -1;
				}
			}
			else if (ch == 'l')
			{
				if (len_state == PRINTF_NORMAL_INT)
				{
					len_state = PRINTF_LONG_INT;
				}
				else if (len_state == PRINTF_LONG_INT)
				{
					len_state = PRINTF_LONG_LONG_INT;
				}
				else
				{
					return -1;
				}
			}
			else
			{
				if ((ch == 'd') || (ch == 'i'))
				{
					// print integer
					int32_t num;
					if (len_state == PRINTF_CHAR_INT)
					{
						num = (int32_t)(int8_t)(*args);
					}
					else if (len_state == PRINTF_SHORT_INT)
					{
						num = (int32_t)(int16_t)(*args);
					}
					else
					{
						num = *args;
					}
					if (num == 0)
					{
						putc('0');
					}
					else
					{
						uint8_t num_arr[21];
						uint8_t sign = '+';
						int32_t curr_ind = 0;
						if (num < 0)
						{
							num = -num;
							sign = '-';
						}
						while (num != 0)
						{
							num_arr[curr_ind] = num % 10;
							num /= 10;
							curr_ind++;
						}
						curr_ind--;

						if (sign == '-')
						{
							putc('-');
						}
						while (curr_ind >= 0)
						{
							putc('0' + num_arr[curr_ind]);
							curr_ind--;
						}
					}
					args++;
					state = PRINTF_NORMAL_STATE;
				}
				else if (ch == 'u')
				{
					// print unsigned integer
					uint32_t num;
					if (len_state == PRINTF_CHAR_INT)
					{
						num = (uint32_t)(uint8_t)(*args);
					}
					else if (len_state == PRINTF_SHORT_INT)
					{
						num = (uint32_t)(uint16_t)(*args);
					}
					else
					{
						num = *args;
					}
					if (num == 0)
					{
						putc('0');
					}
					else
					{
						uint8_t num_arr[21];
						int32_t curr_ind = 0;

						while (num != 0)
						{
							num_arr[curr_ind] = num % 10;
							num /= 10;
							curr_ind++;
						}
						curr_ind--;

						while (curr_ind >= 0)
						{
							putc('0' + num_arr[curr_ind]);
							curr_ind--;
						}
					}
					args++;
					state = PRINTF_NORMAL_STATE;
				}
				else if (ch == 'b')
				{
					// print binary representation
					int32_t num;
					if (len_state == PRINTF_CHAR_INT)
					{
						num = (int32_t)(int8_t)(*args);
					}
					else if (len_state == PRINTF_SHORT_INT)
					{
						num = (int32_t)(int16_t)(*args);
					}
					else
					{
						num = *args;
					}
					if (num == 0)
					{
						putc('0');
					}
					else
					{
						int8_t num_arr[65];
						uint8_t sign = '+';
						int32_t curr_ind = 0;
						if (num < 0)
						{
							num = -num;
							sign = '-';
						}
						while (num != 0)
						{
							num_arr[curr_ind] = num % 2;
							num /= 2;
							curr_ind++;
						}
						curr_ind--;

						if (sign == '-')
						{
							putc('-');
						}
						while (curr_ind >= 0)
						{
							putc('0' + num_arr[curr_ind]);
							curr_ind--;
						}
					}
					putc('b');
					args++;
					state = PRINTF_NORMAL_STATE;
				}
				else if (ch == 'c')
				{	
					if(len_state != PRINTF_NORMAL_INT){
						return -1;
					}
					// print character
					uint8_t ch = (uint8_t)(*args);
					putc(ch);

					args++;
					state = PRINTF_NORMAL_STATE;
				}
				else if (ch == 's')
				{	
					if(len_state != PRINTF_NORMAL_INT){
						return -1;
					}
					// print string
					const char *str = (const char *)(*args);
					puts(str);

					args++;
					state = PRINTF_NORMAL_STATE;
				}
				else if (ch == 'x')
				{
					// print hex number
					int32_t num;
					if (len_state == PRINTF_CHAR_INT)
					{
						num = (int32_t)(int8_t)(*args);
					}
					else if (len_state == PRINTF_SHORT_INT)
					{
						num = (int32_t)(int16_t)(*args);
					}
					else
					{
						num = *args;
					}

					putc('0');
					putc('x');

					if (num == 0)
					{
						putc('0');
					}
					else
					{
						uint8_t num_arr[21];

						uint8_t sign = '+';
						int32_t curr_ind = 0;
						if (num < 0)
						{
							num *= -1;
							sign = '-';
						}
						while (num != 0)
						{
							num_arr[curr_ind] = num % 16;
							num /= 16;
							curr_ind++;
						}

						curr_ind--;

						if (sign == '-')
						{
							putc('-');
						}
						while (curr_ind >= 0)
						{
							uint8_t rem = num_arr[curr_ind];
							uint8_t char_to_print;
							switch (rem)
							{
							case 10:
								char_to_print = 'A';
								break;
							case 11:
								char_to_print = 'B';
								break;
							case 12:
								char_to_print = 'C';
								break;
							case 13:
								char_to_print = 'D';
								break;
							case 14:
								char_to_print = 'E';
								break;
							case 15:
								char_to_print = 'F';
								break;
							default:
								char_to_print = '0' + rem;
								break;
							}
							putc(char_to_print);
							curr_ind--;
						}
					}
					args++;
					state = PRINTF_NORMAL_STATE;
				}
				else if (ch == 'p')
				{	
					if(len_state != PRINTF_NORMAL_INT){
						return -1;
					}
					// print hex number
					uint32_t num = (uint32_t)(*args);

					putc('0');
					putc('x');

					if (num == 0)
					{
						putc('0');
					}
					else
					{
						uint8_t num_arr[21];
						int32_t curr_ind = 0;
						
						while (num != 0)
						{
							num_arr[curr_ind] = num % 16;
							num /= 16;
							curr_ind++;
						}

						curr_ind--;

						while (curr_ind >= 0)
						{
							uint8_t rem = num_arr[curr_ind];
							uint8_t char_to_print;
							switch (rem)
							{
							case 10:
								char_to_print = 'A';
								break;
							case 11:
								char_to_print = 'B';
								break;
							case 12:
								char_to_print = 'C';
								break;
							case 13:
								char_to_print = 'D';
								break;
							case 14:
								char_to_print = 'E';
								break;
							case 15:
								char_to_print = 'F';
								break;
							default:
								char_to_print = '0' + rem;
								break;
							}
							putc(char_to_print);
							curr_ind--;
						}
					}
					args++;
					state = PRINTF_NORMAL_STATE;
				}
				len_state = PRINTF_NORMAL_INT;
			}
		}
		fmt++;
	}

	return 0;
}
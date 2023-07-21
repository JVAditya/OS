#include <stdio.h>
#include <arch/io.h>
#include <stdbool.h>

const uint32_t SCREEN_WIDTH = 80;
const uint32_t SCREEN_HEIGHT = 25;
const uint8_t DEFAULT_COLOR = 0x7;

#define FB_DATA_PORT 0x3D5
#define FB_STATUS_PORT 0x3D4

uint8_t* g_ScreenBuffer = (uint8_t*)(0xB8000);
int g_ScreenX = 0, g_ScreenY = 0;

void put_chr(int x, int y, uint8_t c){
    /*
        Places character c at (x, y) on framebuffer. Each position has 2 bytes - character, color
        @param x: x offset of character
        @param y: y offset of character
        @param c: character to be written
    */
    g_ScreenBuffer[2 * (x + y * SCREEN_WIDTH)] = c;
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
    int pos = x + y * SCREEN_WIDTH;
    outb(FB_STATUS_PORT, 0x0F);     // tell the port that lower 8 bits are being sent
    outb(FB_DATA_PORT, (uint8_t)(pos & 0xFF));
    outb(FB_STATUS_PORT, 0x0E);     // tell the port that higher 8 bits are being sent
    outb(FB_DATA_PORT, (uint8_t)((pos >> 8) & 0xFF));

    g_ScreenX = x;
    g_ScreenY = y;
}

void clrscr(){
    
    for(uint32_t y = 0; y < SCREEN_HEIGHT; y++){
        for(uint32_t x = 0; x < SCREEN_WIDTH; x++){
            put_chr(x, y, '\0');
            put_color(x, y, DEFAULT_COLOR);
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
    for(uint32_t y = num_lines; y < SCREEN_HEIGHT; y++){
        for(uint32_t x = 0; x < SCREEN_WIDTH; x++){
            uint8_t chr = get_chr(x, y);
            put_chr(x, y, '\0');
            
            uint8_t color = get_color(x, y);
            put_color(x, y, DEFAULT_COLOR);

            put_chr(x, y - num_lines, chr);
            put_color(x, y - num_lines, color);
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
            if(g_ScreenY < (int)SCREEN_HEIGHT-1){
                g_ScreenY++;
            }
            else{
                scrolldown(1);
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
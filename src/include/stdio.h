#pragma once
#include <stdint.h>
#include <stdbool.h>

/*
    Clears the screen and moves the cursor to (0, 0)
*/
void clrscr();

/*
    Prints a character at cursor location on the framebuffer and shifts the cursor forward.
    @param c: character to be printed
*/
void putc(char c);

/*
    Prints a string at cursor location on the framebuffer and shifts the cursor forward.
    @param str: null terminated string to be printed
*/
void puts(const char* str);
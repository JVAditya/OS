#pragma once
#include <arch/io.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define VGA_BUFFER_ADDR 0xC0000000

#define FB_DATA_PORT 0x3D5
#define FB_STATUS_PORT 0x3D4

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

/*
    Prints a format string at cursor location on the framebuffer and shifts the cursor forward.
    @param str: null terminated string to be printed
    @returns: 0 always

    https://cplusplus.com/reference/cstdio/printf/
*/
#define PRINTF_NORMAL_STATE     0
#define PRINTF_FORMAT_STATE     1
#define PRINTF_NORMAL_INT       2
#define PRINTF_CHAR_INT         3
#define PRINTF_SHORT_INT        4
#define PRINTF_LONG_INT         5
#define PRINTF_LONG_LONG_INT    6

int __attribute__((cdecl)) printf(const char* fmt, ...);

typedef struct {
    uint32_t framebuffer_addr_low;
    uint32_t framebuffer_addr_high;
    uint32_t framebuffer_pitch;
    uint32_t framebuffer_width;
    uint32_t framebuffer_height;
    uint8_t framebuffer_bpp;
    uint8_t framebuffer_type;
    uint8_t color_info[6];
} __attribute__((packed)) FrameBufferInfo;

/*
    Makes the framebuffer available to C code by passing the pointer to framebuffer data in multiboot info table.
    Also calls graphics_pagr_entry function to map the framebuffer's physical address
    This should be done before identity paging is disabled
    @param fb_multiboot_info : Address of framebuffer data as returned in multiboot info table (before disabling identity paging)
*/
void initialize_framebuffer(uint8_t* fb_multiboot_info);

#define PSF1_MAGIC0     0x36
#define PSF1_MAGIC1     0x04

#define PSF1_MODE512    0x01
#define PSF1_MODEHASTAB 0x02
#define PSF1_MODEHASSEQ 0x04
#define PSF1_MAXMODE    0x05

#define PSF1_SEPARATOR  0xFFFF
#define PSF1_STARTSEQ   0xFFFE

typedef struct {
    uint16_t magic;     /* Magic number */
    uint8_t mode;         /* PSF font mode */
    uint8_t charsize;     /* Character size */
} __attribute__((packed)) PSF1_HEADER;

/*
    Loads the font file and initializes the array required for rendering the PSF fonnt.
*/
void initialize_psf();
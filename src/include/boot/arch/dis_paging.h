#pragma once

/*
    Disables identity paging which was initially done so that instructions are executed properly. Do this once
    the multiboot info table is read
*/
void __attribute__((cdecl)) disable_identity_paging();

/*
    Create page directory entries for graphics framebuffer. 4 MB pages are allocated in this case unlike 4 KB pages.
    A total of 8 MB is allocated just in case 1920x1080x32 resolution is used.

    @param framebuff_addr : Physical address off framebuffer (as returned by multiboot info table)
    @param width : Width of the framebuffer (640 by default)
    @param height : Height of the framebuffer (480 by default)
    @param depth : Depth of each pixel (32 bits by default)
*/
void __attribute__((cdecl)) graphics_page_entry(uint32_t framebuff_addr, uint32_t width, uint32_t height, uint32_t depth);
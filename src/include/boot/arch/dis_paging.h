#pragma once

void __attribute__((cdecl)) disable_identity_paging();
void __attribute__((cdecl)) graphics_page_entry(uint32_t framebuff_addr, uint32_t width, uint32_t height, uint32_t depth);
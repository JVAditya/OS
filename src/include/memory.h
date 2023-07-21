#pragma once
#include <stdint.h>

/*
    Copies `num` bytes of data from memory pointed to by `src` to memory pointed to by `dst`
    @param dst: Memory destination address
    @param src: Memory source address
    @param num: Number of bytes to copy
*/
void* memcpy(void* dst, const void* src, uint16_t num);

/*
    Sets `num` bytes of data pointed to at by `ptr` to the byte `value`
    @param ptr: Memory address
    @param value: Byte to set memory to
    @param num: Number of bytes to set
*/
void* memset(void* ptr, int value, uint16_t num);

 /*
    Compares two memory segments to find if they are equal or not
    @param ptr1: Starting address of first segment
    @param ptr2: Starting address of second segment
    @param num: Number of bytes to compare
*/
int memcmp(const void* ptr1, const void* ptr2, uint16_t num);
#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

void reinitialize_paging();
uint32_t* get_physical_address(void* virtual_addr);
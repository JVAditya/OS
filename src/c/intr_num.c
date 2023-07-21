#include <intr_num.h>

void put_intr(int interrupt){
    putc((uint8_t)(((char)(interrupt)) + '0'));
}
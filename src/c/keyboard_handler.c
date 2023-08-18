#include <keyboard_handler.h>
#include <stdio.h>
#include <stdbool.h>
#include <arch/testing.h>

bool caps = false;

char caps_char(char c, bool caps){  // for cpps lock key
    if(caps){
        return (c - 32);
    }
    return c;
}

void keyboard_handler(Registers* regs){
    uint8_t scan_code = inb(KBD_DATA_PORT);
    
    if((scan_code >= 0x02) && (scan_code <= 0x0A)){
        // 1-0 numbers
        putc(caps_char('0' + (scan_code-1), false));
    }
    else if(scan_code == 0x0B){
        putc(caps_char('0', false));
    }
    else if(scan_code == 0x0C){
        putc(caps_char('-', false));
    }
    else if(scan_code == 0x0D){
        putc(caps_char('=', false));
    }
    else if(scan_code == 0x0F){
        putc(caps_char('\t', false));
    }
    else if(scan_code == 0x1C){
        putc(caps_char('\n', false));
    }
    else if(scan_code == 0x39){
        putc(caps_char(' ', false));
    }
    else if(scan_code == 0x3A){
        caps = !caps;
    }
    switch(scan_code){
        case 0x10:
            putc(caps_char('q', caps));
            break;
        case 0x11:
            putc(caps_char('w', caps));
            break;    
        case 0x12:
            putc(caps_char('e', caps));
            break;
        case 0x13:
            putc(caps_char('r', caps));
            break;
        case 0x14:
            putc(caps_char('t', caps));
            break;
        case 0x15:
            putc(caps_char('y', caps));
            break;
        case 0x16:
            putc(caps_char('u', caps));
            break;
        case 0x17:
            putc(caps_char('i', caps));
            break;
        case 0x18:
            putc(caps_char('o', caps));
            break;
        case 0x19:
            putc(caps_char('p', caps));
            break;
        case 0x1A:
            putc(caps_char('[', false));
            break;
        case 0x1B:
            putc(caps_char(']', false));
            break;
        case 0x1E:
            putc(caps_char('a', caps));
            break;
        case 0x1F:
            putc(caps_char('s', caps));
            break;
        case 0x20:
            putc(caps_char('d', caps));
            break;
        case 0x21:
            putc(caps_char('f', caps));
            break;
        case 0x22:
            putc(caps_char('g', caps));
            break;
        case 0x23:
            putc(caps_char('h', caps));
            break;
        case 0x24:
            putc(caps_char('j', caps));
            break;
        case 0x25:
            putc(caps_char('k', caps));
            break;
        case 0x26:
            putc(caps_char('l', caps));
            break;
        case 0x27:
            putc(caps_char(';', false));
            break;
        case 0x28:
            putc(caps_char(0x27, false)); // ' character
            break; 
        case 0x29:
            putc(caps_char('`', false));
            break;
        case 0x2B:
            putc(caps_char('\\', false));
            break;
        case 0x2C:
            putc(caps_char('z', caps));
            break;
        case 0x2D:
            putc(caps_char('x', caps));
            break;
        case 0x2E:
            putc(caps_char('c', caps));
            break;
        case 0x2F:
            putc(caps_char('v', caps));
            break;
        case 0x30:
            putc(caps_char('b', caps));
            break;
        case 0x31:
            putc(caps_char('n', caps));
            break;
        case 0x32:
            putc(caps_char('m', caps));
            break;
        case 0x33:
            putc(caps_char(',', false));
            break;
        case 0x34:
            putc(caps_char('.', false));
            break;
        case 0x35:
            putc(caps_char('/', false));
            break;
    }
}

#pragma once
#include <stdint.h>
#include <stdbool.h>

/*
    outb: sends a byte of data to a given port. Defined in io.s
    @param port: I/O port to send data to
    @param data: byte of data to be sent to the port.
*/
void __attribute__((cdecl)) outb(uint16_t port, uint8_t data);

/*
    inb: receives a byte of data from a given port. Defined in io.s
    @param port: I/O port to receive data from
    @returns data: The byte of data received
*/
uint8_t __attribute__((cdecl)) inb(uint16_t port);

/*
    Panicks and halts the processor
*/
void __attribute__((cdecl)) panic();

/*
    Enables interrupts on the CPU
*/
void __attribute__((cdecl)) EnableInterrupts();

/*
    Disables interrupts on the CPU
*/
void __attribute__((cdecl)) DisableInterrupts();

/*
    Cleans the output buffer
*/
void iowait();
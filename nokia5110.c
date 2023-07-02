/* Nokia 5110 LCD AVR Library
 *
 * Copyright (C) 2015 Sergey Denisov.
 * Written by Sergey Denisov aka LittleBuster (DenisovS21@gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version 3
 * of the Licence, or (at your option) any later version.
 *
 * Original library written by SkewPL, http://skew.tk
 */

#include "nokia5110.h"
#include "nokia5110_chars.h"

#include <avr/io.h>

#define SS   PB0
#define MOSI PB3
#define DC   PB1
#define SCK  PB5

static inline void write(uint8_t data) {
    SPDR = data;
    while(!(SPSR & (1<<SPIF)));
}

static inline void command_mode() {
    PORTB &= ~(1<<DC);  // activate command mode
}

static inline void data_mode() {
    PORTB |= (1<<DC);  // activate data mode
}

uint8_t nokia_5110_init() {
    DDRB = (1 << SS) |    // SS
           (1 << MOSI) |  // SCK
           (1 << DC) |    // DC
           (1 << SCK);    // SCK

    SPCR = (1 << SPE) |    // enable SPI
           (1 << MSTR);    // SPI mater mode

    command_mode();

    write(0x21);  // Extended instruction set control (H)
    write(0x13);  // LCD bias mode 1:48
    write(0x06);  // Set temperature coefficient
    write(0xC2);  // Default VOP (3.06 + 66 * 0.06 = 7V)

    write(0x20);  // chip active, horizontal addr, normal control (~H)
    write(0x0C);  // set normal mode

    nokia_5110_clear();

    return 0;
}

void nokia_5110_clear(void) {
    command_mode();

    write(0x80);  // set X address to 0
    write(0x40);  // set Y address to 0 (bank 0)

    data_mode();  // activate data mode

    for(unsigned i = 0;i < 504; i++) {
        write(0x00);  // clear LCD memory
    }
}

void nokia_5110_set_xy(uint8_t x, uint8_t y) {
    command_mode();

    x = (x % 84);
    y = (y % 6);

    write(0x80 | x);
    write(0x40 | y);
}

//Inverts the current screen
void nokia_5110_invert() {
    command_mode();
    write(0x0D);
}

//Noninverts the screen
void nokia_5110_noninvert() { 
    command_mode();
    write(0x0C);
}

//Send ASCII data to the display
void nokia_5110_write(char* data) {
    data_mode();  // activate data mode

    while (*data) {
        char c = *data++;
        for (int j=0; j<5; j++) {
            #if __PGMSPACE_H_
                write(pgm_read_byte(&(CHARSET[c - 0x20][j])));
            #else
                write(CHARSET[c - 0x20][j]);
            #endif
        }
        write(0x00);
    }
}

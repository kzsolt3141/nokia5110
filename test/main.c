/* Nokia 5110 LCD AVR Library example
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

#include <avr/io.h>
#include <util/delay.h>

#include "nokia5110.h"
#include "spi.h"

static inline void init_spi_master() {
    SPI_init_master(SPI_PS_4);
}

int main(void) {
    if (nokia_5110_init(init_spi_master, SPI_RW_byte)) return 1;

    nokia_5110_write("Crazy");
     _delay_ms(1000);
     nokia_5110_invert();
     _delay_ms(1000);

    nokia_5110_set_xy(10, 4);
    nokia_5110_write("Monkey");
     _delay_ms(1000);
     nokia_5110_noninvert();

     _delay_ms(1000);

    nokia_5110_clear();
}
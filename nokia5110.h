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

#ifndef __NOKIA_5110_H__
#define __NOKIA_5110_H__

#include <stdint.h>
#include <stddef.h>

uint8_t nokia_5110_init();

void nokia_5110_clear(void);

void nokia_5110_set_xy(uint8_t x, uint8_t y);

void nokia_5110_invert();

void nokia_5110_noninvert();

void nokia_5110_write(char* data);

#endif
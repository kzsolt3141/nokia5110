#ifndef __NOKIA_5110_H__
#define __NOKIA_5110_H__

#include <stdint.h>
#include <stddef.h>

/**
 * Configure SPI as master to communicate with Nokia 5110 display
 */
uint8_t nokia_5110_init();

/**
 * Clear the display and the display memory. This function requires a working SPI connection.
 */
void nokia_5110_clear();

/**
 * Set cursor to X column and Y bank. Please not that the display has 84 colums and 6 banks
 * @param[in] x cursor column
 * @param[in] y cursor bank
 */
void nokia_5110_set_xy(uint8_t x, uint8_t y);

/**
 * Invert display B-> W and W -> B
 */
void nokia_5110_invert();

/**
 * Send character array to the display until 0 character found.
 * With each character sent the cursor position will be incremented with one character. (5 columns)
 * @param[in] data character array ending with 0
 */
void nokia_5110_write(char* data);

#endif  // __NOKIA_5110_H__

#ifndef __NOKIA_5110_H__
#define __NOKIA_5110_H__

#include <stdint.h>
#include <stddef.h>

/**
 * SPI interrupt callback type
 * @param[inout] ctx user data for interrupt callback
 * When SPI ISR occurs SPI_isr_cb will be called with ctx as parameter
 */
typedef void (*SPI_isr_cb)(void* ctx);

/**
 * Configure SPI as master but with main SS pin set as input. Multi-master mode compatible.
 * SS pint to the slave is PB0.
 * Initializes display.
 * @param[in] SPI_cb SPI interrupt callback - will be used when an other master requires data
 * @param[in] SPI_cb_ctx SPI interrupt callback context
 * When ISR occurs SPI_isr_cb will be called with ctx as parameter
 * 
 * @return 0 - OK
 *         1 - failed
 */
uint8_t nokia_5110_init(SPI_isr_cb SPI_cb, void* SPI_cb_ctx);

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

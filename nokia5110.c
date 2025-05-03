#include "nokia5110.h"
#include "nokia5110_chars.h"

#include "spi/spi.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#define RST  PB0
#define DC   PB1

// seet spi/spi.c for pinout
// #define SS   PB2
// #define MOSI PB3
// #define MISO PB4
// #define SCK  PB5

static uint8_t     invert = 0x0C;  // keep in mind the current invert status

// activate display command mode by pulling DC pin down
static inline void command_mode() {
    PORTB &= ~(1<<DC);
}

// activate display data mode by setting DC pin
static inline void data_mode() {
    PORTB |= (1<<DC);  // activate data mode
}

static void display_init() {
    PORTB &= ~(1 << RST);  // display HW reset (will not clear the buffer)
    PORTB |= (1 << RST);   // disable display HW reset

    command_mode();

    SPI_RW_byte(0x21);  // Extended instruction set control (H)
    SPI_RW_byte(0x13);  // LCD bias mode 1:48
    SPI_RW_byte(0x06);  // Set temperature coefficient
    SPI_RW_byte(0xC2);  // Default VOP (3.06 + 66 * 0.06 = 7V)

    SPI_RW_byte(0x20);  // chip active, horizontal addr, normal control (~H)
    SPI_RW_byte(0x0C);  // set normal mode
}

//////////////////////////////////////////////////////////

uint8_t nokia_5110_init() {
    SPI_init_master(SPI_PS_4);
    cli();

    DDRB |= (1 << DC) |  // data/control pin 
            (1 << RST);   // display reset pin

    display_init();
    nokia_5110_clear();

    sei();

    return 0;
}

void nokia_5110_clear() {
    command_mode();

    SPI_RW_byte(0x80);  // set X address to 0
    SPI_RW_byte(0x40);  // set Y address to 0 (bank 0)

    data_mode();  // activate data mode

    for(unsigned i = 0;i < 504; i++) {
        SPI_RW_byte(0x00);  // clear LCD memory
    }
}

void nokia_5110_set_xy(uint8_t x, uint8_t y) {
    command_mode();

    x = (x % 84);
    y = (y % 6);

    SPI_RW_byte(0x80 | x);
    SPI_RW_byte(0x40 | y);
}

//Inverts the current screen
void nokia_5110_invert() {
    invert ^= 0x01;
    command_mode();
    SPI_RW_byte(invert);
}

//Send ASCII data to the display
void nokia_5110_write(char* data) {
    data_mode();  // activate data mode

    while (*data) {
        char c = *data++;
        for (int j=0; j<5; j++) {
            #if __PGMSPACE_H_
            SPI_RW_byte(pgm_read_byte(&(CHARSET[c - 0x20][j])));
            #else
            SPI_RW_byte(CHARSET[c - 0x20][j]);
            #endif
        }
        SPI_RW_byte(0x00);
    }
}
#include "nokia5110.h"
#include "nokia5110_chars.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#define SS   PB0
#define MOSI PB3
#define MISO PB4
#define DC   PB1
#define SCK  PB5
// #define MASTER_SS PB2
#define DISP_RTS PD7
#define INT0_PIN PD2

static SPI_isr_cb  SPI_cb_     = NULL;
static void       *SPI_cb_ctx_ = NULL;

static uint8_t     invert = 0x0C;  // keep in mind the current invert status

// send data in master mode to the slave, wait until the transaction is ready.
static inline void write(uint8_t data) {
    if ((SPCR & (1 << MSTR)) == 0) return;  // don't send display data when MCU is in slave mode

    SPDR = data;
    while(!(SPSR & (1<<SPIF)));
}

// activate display command mode by pulling DC pin down
static inline void command_mode() {
    PORTB &= ~(1<<DC);
}

// activate display data mode by pulling DC pin down
static inline void data_mode() {
    PORTB |= (1<<DC);  // activate data mode
}

static void display_init() {
    PORTD &= ~(1 << DISP_RTS);  // display HW reset (will not clear the buffer)
    PORTD |= (1 << DISP_RTS);  // disable display HW reset

    command_mode();

    write(0x21);  // Extended instruction set control (H)
    write(0x13);  // LCD bias mode 1:48
    write(0x06);  // Set temperature coefficient
    write(0xC2);  // Default VOP (3.06 + 66 * 0.06 = 7V)

    write(0x20);  // chip active, horizontal addr, normal control (~H)
    write(0x0C);  // set normal mode
}

//////////////////////////////////////////////////////////

uint8_t nokia_5110_init(SPI_isr_cb SPI_cb, void* SPI_cb_ctx) {
    if (NULL == SPI_cb || NULL == SPI_cb_ctx) return 1;

    cli();

    DDRB = (1 << SS) |
           (1 << MOSI) |
           (1 << MISO) |  // to be able to send data when it is in slave mode
           (1 << DC) |
           (1 << SCK);

    DDRD |= (1 << DISP_RTS);  // conn
    DDRD &= ~(1 << INT0_PIN);  // input for INT0

    MCUCR |= (1 << ISC00) |
             (1 << ISC01);  // general interrupt ISR0 on rising edge

    GICR |= (1 << INT0);  // enable ISR0

    SPCR = (1 << SPE) |    // enable SPI
           (1 << MSTR);    // SPI mater mode

    SPI_cb_ = SPI_cb;
    SPI_cb_ctx_ = SPI_cb_ctx;
    SPCR |= (1 << SPIE);  // SPI interrupt enable

    display_init();
    nokia_5110_clear();

    sei();

    return 0;
}

void nokia_5110_clear() {
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
    invert ^= 0x01;
    command_mode();
    write(invert);
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

//////////////////////////////////////////////////////////

// SPI interrupt will be defined in user application
ISR(SPI_STC_vect) {
    SPI_cb_(SPI_cb_ctx_);
}

// INT0 external interrupt to bring back SPI to master mode
ISR(INT0_vect) {
    SPCR |= (1 << MSTR);    // put back SPI in mater mode
    display_init();
}

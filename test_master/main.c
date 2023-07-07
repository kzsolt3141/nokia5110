/*
 * adc.c
 *
 * Created: 12/17/2019 12:09:35 AM
 * Author : Zsolt
 */ 

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#include "spi.h"
#include "uart.h"

int main(void) {
    // UART INIT
    //-------------------------------
    const uint16_t baud_rate = 38400;

    USART_init(baud_rate);

    printf("Init Done UART baud: %u\n", (uint16_t)baud_rate);
    //-------------------------------

    // SPI INIT
    //-------------------------------

    regiter_SPI_isr_cb(NULL, NULL);

    // PORTC INIT
    //-------------------------------
    DDRC |= (1 << PC0);
    PORTC |= (1 << PC0);

    while (1) {
        SPI_init_master(SPI_PS_16);
        printf("Init Done SPI\n");
        for (uint8_t i = 0; i < 10; i++) {
            PORTC &= ~(1 << PC0);
            printf("request %d\n", i);
            uint8_t rcv = SPI_RW_byte(i);
            printf("receive %d\n", rcv);
            _delay_ms(1000);
        }

        SPCR = 0;
        DDRB = 0;

        PORTC |= (1 << PC0);

        for (uint8_t i = 0; i < 10; i++) {
            _delay_ms(1000);
        }
    };

    return 0;
}

#include <avr/io.h>
#include <util/delay.h>

#include "nokia5110.h"
#include "uart.h"
#include <stdio.h>

int main(void) {
    const uint16_t baud_rate = 38400;
    USART_init(baud_rate);

    printf("Init Done UART baud: %u\n", (uint16_t)baud_rate);

    _delay_ms(500);
    nokia_5110_init();
    _delay_ms(500);

    nokia_5110_write("Crazy");
     _delay_ms(1000);
     nokia_5110_invert();
     _delay_ms(1000);

    nokia_5110_set_xy(10, 2);
    nokia_5110_write("Monkey");
     _delay_ms(1000);
     nokia_5110_invert();

     uint8_t xpos = 0;
     while (1) {
        if (xpos % 4 == 0)
        nokia_5110_invert();
        nokia_5110_set_xy(xpos, 4);
        nokia_5110_write("Dance");
        _delay_ms(100);
        nokia_5110_set_xy(xpos, 4);
        nokia_5110_write("        ");
       
        xpos = xpos > 25 ? 0 : xpos + 1;
     }
}

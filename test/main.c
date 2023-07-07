#include <avr/io.h>
#include <util/delay.h>

#include "nokia5110.h"
#include "uart.h"
#include <stdio.h>

typedef struct SPI_cb_ctx_t {
    uint8_t data;
    int intr_cnt;
}SPI_cb_ctx;

SPI_cb_ctx ctx = {0};

static void SPI_cb_handle(void* ctx) {
    SPI_cb_ctx* t_ctx = (SPI_cb_ctx*)ctx;

    t_ctx->data = SPDR;
    t_ctx->intr_cnt++;


    if (SPCR & (1 << MSTR)) {
        printf("M\n");
    } else {
        printf("S %02x\n", t_ctx->data);
        SPDR = t_ctx->data;
    }
}

int main(void) {
    const uint16_t baud_rate = 38400;
    USART_init(baud_rate);

    printf("Init Done UART baud: %u\n", (uint16_t)baud_rate);

    nokia_5110_init(SPI_cb_handle, &ctx);

    nokia_5110_write("Crazy");
     _delay_ms(1000);
     nokia_5110_invert();
     _delay_ms(1000);

    nokia_5110_set_xy(10, 4);
    nokia_5110_write("Monkey");
     _delay_ms(1000);
     nokia_5110_invert();

     while (1) {
        nokia_5110_invert();
        _delay_ms(300);
     }
}

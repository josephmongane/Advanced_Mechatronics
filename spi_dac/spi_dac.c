#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"

#define PICO_DEFAULT_SPI_RX_PIN 16
#define PICO_DEFAULT_SPI_SCK_PIN 18
#define PICO_DEFAULT_SPI_TX_PIN 19
#define PIN_CS 17
#define SPI_PORT spi0

void write_dac(int chan, float voltage); 
static inline void cs_select(uint cs_pin);
static inline void cs_deselect(uint cs_pin);


// Add the hardware SPI initializations 


int main()
{
    stdio_init_all();

    // initializing the SPI coms 

    spi_init(spi_default, 1000 * 1000); // the baud, or bits per second

    gpio_set_function(PICO_DEFAULT_SPI_RX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI);

    while (true) {
        // calll the write dac function.
        float t = 0;
        float freq = 2;
        t = t + 0.1;
        float voltage = (sin(2 * 3.14159 * freq * t) + 1) * 3.3 / 2.0; 
        write_dac(1, voltage); 
        sleep_ms(10);
    }
}

void write_dac(int chan, float voltage) {
    uint8_t data[2];
    data[0] = 0b01110000; 
    // Chosing the channel and the chip
    data[0] = ( data[0] | ((chan & 0b1)<<7) ); 

    // converting a float to a bianary quant and 
    uint16_t bit_volt = voltage*1023 / 3.3;
    data[0] = ( data[0] | ((bit_volt>>6) & 0b0000001111) );

    data[1] = (bit_volt << 2)& 0xFF; 

    
    cs_select(PIN_CS);
    spi_write_blocking(SPI_PORT, data, 2); // where data is a uint8_t array with length len
    cs_deselect(PIN_CS);
}

static inline void cs_select(uint cs_pin) {
    asm volatile("nop \n nop \n nop"); // FIXME Not
    gpio_put(cs_pin, 0);
    asm volatile("nop \n nop \n nop"); // FIXME Not
}

static inline void cs_deselect(uint cs_pin) {
    asm volatile("nop \n nop \n nop"); // FIXME Not
    gpio_put(cs_pin, 1);
    asm volatile("nop \n nop \n nop"); // FIXME Not
}
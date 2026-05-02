#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"

#define PICO_DEFAULT_SPI_RX_PIN 16
#define PICO_DEFAULT_SPI_SCK_PIN 18
#define PICO_DEFAULT_SPI_TX_PIN 19
#define RAM_CS 15
#define DAC_CS 17
#define SPI_PORT spi0

#define READ_INSTRUCT 0b00000011
#define WRITE_INSTRUCT 0b00000010
#define MODE_SET 0b00000001
#define BYTE_OPP 0b00000000
#define SEQ_OPP 0b01000000
#define PAGE_OPP 0b10000000

void write_dac(int data_index); 
static inline void cs_select(uint cs_pin);
static inline void cs_deselect(uint cs_pin);

void spi_ram_init();
void spi_ram_read(uint16_t addr, uint8_t *data_int, int len); 
void spi_ram_write(uint16_t addr, uint8_t *data, int len); 
void write_wave(); 

union FloatInt {
    float f;
    uint16_t i;
};

// Add the hardware SPI initializations 


int main()
{
    stdio_init_all();

    while(!stdio_usb_connected()); 
    spi_ram_init();
    write_wave(); 
    while (true) {
        //calll the write dac function.
        uint8_t data_in[1024]; 
        int i; 
        for (i = 0; i < 1024; i++) {
            spi_ram_read(i, data_in, 5); 
            sleep_ms(10);
        }


    }
}

void write_dac(int data_index) {
    uint8_t val[2]; 
    spi_ram_read(data_index, val, 2);
    
    cs_select(DAC_CS);
    spi_write_blocking(SPI_PORT, val, 2); // where data is a uint8_t array with length len
    cs_deselect(DAC_CS);

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

void spi_ram_init() {
    // initializes the spi ram chip 
        // initializing the SPI coms 
    spi_init(spi_default, 1000 * 1000); // the baud, or bits per second

    gpio_init(RAM_CS); 
    gpio_set_dir(RAM_CS, GPIO_OUT);
    gpio_init(DAC_CS); 
    gpio_set_dir(DAC_CS, GPIO_OUT);

    gpio_put(DAC_CS, 1);
    gpio_put(RAM_CS, 1);
    
    gpio_set_function(PICO_DEFAULT_SPI_RX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI);


    uint8_t data[2];
    int len = 2;
    data[0] = MODE_SET;
    data[1] = SEQ_OPP;
    cs_select(RAM_CS);
    spi_write_blocking(SPI_PORT, data, len); // where data is a uint8_t array with length len
    cs_deselect(RAM_CS);
}

void spi_ram_read(uint16_t addr, uint8_t *data_in, int len) { //not using len
    // read the data in the chip 
    uint16_t read_data; 
    uint8_t packet[5];
    packet[0] = READ_INSTRUCT;
    packet[1] = addr >> 8;
    packet[2] = addr & 0xFF; 
    packet[3] = 0;
    packet[4] = 0; 
    uint8_t report[5]; 
    cs_select(RAM_CS);
    spi_write_read_blocking(SPI_PORT, packet, report, 5); // where data is a uint8_t array with length len
    cs_deselect(RAM_CS);
    data_in[0] = report[3];
    data_in[1] = report[4];

    read_data = (data_in[0] << 8);
    read_data = (read_data | data_in[1]);
    printf("%d\n", read_data);
}

void spi_ram_write(uint16_t addr, uint8_t *data, int len) {
    // write data to the chip 
    uint8_t write_package[5];
    write_package[0] = WRITE_INSTRUCT;
    write_package[1] = addr>>8; 
    write_package[2] = addr&0xFF;
    write_package[3] = data[0];
    write_package[4] = data[1]; 

    // Send the data
    cs_select(RAM_CS);
    spi_write_blocking(SPI_PORT, write_package, 5); // where data is a uint8_t array with length len
    cs_deselect(RAM_CS);
}

void write_wave() {
    int i; 
    float voltage; 
    uint16_t volts;
    uint8_t data[2]; 
    uint16_t addr = 0;
    uint8_t chan = 0b0; 

    for (i = 0; i < 1024; i++) {
        uint16_t data_16  = ((chan&0b1)<<15); 
        data_16 = data_16 & (0b111<<12); 

        voltage = (sin(2 * 3.141 * i / 1024) + 1) * 511.5; 

        volts = (int)voltage;
        data_16 = data_16 | (0b111111111111 & volts); 

        data[0] = data_16 >> 8; 
        data[1] = data_16 & 0xFF; 

        spi_ram_write(addr, data, 2); 
        addr = addr + 2; 
    }
}
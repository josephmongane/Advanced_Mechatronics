#include "HW3.h"


int main()
{
    stdio_init_all();

    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400*1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    uint8_t buf[2];
    uint8_t read_reg[2];


    // For more examples of I2C use see https://github.com/raspberrypi/pico-examples/tree/master/i2c

    // Initialize the pins as GPIO outputs
    init_mpc();

    // Send data 
    buf[0] = 0x0A; 
    buf[1] = 0b10000000; 
    i2c_write_blocking(i2c_default, ADDR, buf, 2, false); 


    while (true) {
        int result = read_mpc(0);
        write_mpc(6, 1);
        sleep_ms(500); 
        write_mpc(6, 0);
        printf("Value %d\n", result);
        sleep_ms(500); 
    }
}

void init_mpc() {
    uint8_t buf[2]; 
    buf[0] = 0x0;
    buf[1] = 0b00111111;
    i2c_write_blocking(i2c_default, ADDR, buf, 2, false); 
}

int read_mpc(int pin) {
    int out; 
    uint8_t read_reg[2];
    read_reg[0] = 0x09;
    uint8_t pin_bit = 0b1 << pin;
    i2c_write_blocking(i2c_default, ADDR, &read_reg[0], 1, true);  // true to keep host control of bus
    i2c_read_blocking(i2c_default, ADDR, &read_reg[1], 1, false);  // false - finished with bus
    out = (pin_bit & read_reg[1]) >> pin;
    return out;
}

void write_mpc(int pin, int value) {
    uint8_t write_buf[2];
    write_buf[0] = 0x0A; 
    i2c_write_blocking(i2c_default, ADDR, &write_buf[0], 1, true);  // true to keep host control of bus
    i2c_read_blocking(i2c_default, ADDR, &write_buf[1], 1, false);  // false - finished with bus
    write_buf[1] = (write_buf[1]) | (value << pin); 
    i2c_write_blocking(i2c_default, ADDR, write_buf, 2, false); 
}


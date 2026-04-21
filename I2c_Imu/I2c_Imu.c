#include "library.h"

// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments

int main()
{
    stdio_init_all();

    uint8_t imu_data[2]; 
    uint8_t imu_register[2];
    imu_register[0] = WHO_AM_I;


    i2c_write_blocking(i2c_default, IMU_ADDR, imu_register, 1, true);  // true to keep host control of bus
    i2c_read_blocking(i2c_default, IMU_ADDR, imu_data, 1, false);  // false - finished with bus

    while (true) {
        printf("Hello, world!\n");
        sleep_ms(1000);
    }
}

void innit_imu() {
    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400*1000);
    
    gpio_set_function(I2C_IMU_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_IMU_SCL, GPIO_FUNC_I2C);
}
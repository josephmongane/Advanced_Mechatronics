#include "library.h"

// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments

int main()
{
    stdio_init_all();
    while(!stdio_usb_connected()); 
    uint8_t output;
    uint8_t imu_data[14]; 
    float combined_data[7];
    sleep_ms(100);
    output = innit_imu();
    printf("output = %d\n", output); 
    int i; 

    while (true) {
        gpio_put(16, 1);
        sleep_ms(1000);
        gpio_put(16, 0);
        sleep_ms(1000); 
        imu_data[0] = ACCEL_XOUT_H; 
        i2c_write_blocking(i2c_default, IMU_ADDR, &imu_data[0], 1, true); 
        i2c_read_blocking(i2c_default, IMU_ADDR, &imu_data[1], 14, false);  
        combine_data(imu_data, combined_data);
        for (i = 0; i < 7; i++) {
            printf("%f\n", combined_data[i]);
        }
    }
}

uint8_t innit_imu() {
    // I2C Initialisation. Using it at 400Khz.
    gpio_init(16); 
    gpio_set_dir(16, GPIO_OUT);

    i2c_init(I2C_PORT, 400*1000);
    
    gpio_set_function(I2C_IMU_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_IMU_SCL, GPIO_FUNC_I2C);

    uint8_t imu_register[3];
    uint8_t imu_data[3]; 
    imu_register[0] = PWR_MGMT_1;
    imu_register[1] = 0b0;

    i2c_write_blocking(i2c_default, IMU_ADDR, imu_register, 2, false); 
     // true to keep host control of bus

     
    imu_register[0] = GYRO_CONFIG;
    imu_register[1] = 0b11111000;
    imu_register[2] = 0b11100000;

    i2c_write_blocking(i2c_default, IMU_ADDR, imu_register, 3, false); 

    imu_data[0] = WHO_AM_I;

    i2c_write_blocking(i2c_default, IMU_ADDR, &imu_data[0], 1, true); 
    i2c_read_blocking(i2c_default, IMU_ADDR, &imu_data[1], 1, false);  
    // false - finished with bus

    return imu_data[1];
}

void combine_data(uint8_t *data_array, float *clean_data) {
    uint16_t combined[7];
    int i; 
    for (i = 0; i < 14; i += 2) {
        combined[i/2] = ((data_array[i]<<8) | data_array[i+1]);
    }
    
    for (i = 0; i < 7; i++) {
        if (i < 3) {
            clean_data[i] = combined[i]*0.000061;
        }
        if (3 <= i < 4) {
            clean_data[i] = combined[i]/340.0 + 36.53;
        }
        if (i >= 4) {
            clean_data[i] = combined[i]*0.007360;
        }
    }
    
}
#ifndef OLED_DISPLAY_H

#include <string.h> // for memset
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "ssd1306.h"

// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define I2C_SCREEN_PORT i2c1
#define I2C_SCREEN_SDA 2
#define I2C_SCREEN_SCL 3

void init_screen();

#endif
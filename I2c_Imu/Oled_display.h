#ifndef OLED_DISPLAY_H

#include <string.h> // for memset
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "ssd1306.h"

// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define I2C_SCREEN_PORT i2c0
#define I2C_SCREEN_SDA 8
#define I2C_SCREEN_SCL 9

void init_screen();
void draw_line(float x_accel, float y_accel);

#endif
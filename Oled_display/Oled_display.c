#include <stdio.h>
#include "Oled_display.h"


int main()
{
    stdio_init_all();
    init_screen();

    while (true) {
        ssd1306_drawPixel(2, 2, 1);
        ssd1306_update();
        sleep_ms(1000); 
        ssd1306_clear();
        ssd1306_update();
        sleep_ms(1000);
    }
}

void init_screen(void) {
    gpio_init(16); 
    gpio_set_dir(16, GPIO_OUT);

    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400*1000);
    
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    // For more examples of I2C use see https://github.com/raspberrypi/pico-examples/tree/master/i2c

    ssd1306_setup();
}
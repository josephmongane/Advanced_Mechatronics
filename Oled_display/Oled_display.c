#include "Oled_display.h"

char text[25]; 
char text_time[25];
uint16_t result; 
float voltage = 0;
unsigned int time;
unsigned int prev_time;

int main()
{
    stdio_init_all();
    init_screen();
    sprintf(text, "Test...");
    adc_init();
    adc_gpio_init(26);
    adc_select_input(0); 

    while(!stdio_usb_connected());
    prev_time = to_us_since_boot(get_absolute_time());  
    while (true) {
        result = adc_read(); 
        time = to_us_since_boot(get_absolute_time());  
        sprintf(text_time, "FPS = %.4f", 1/((time - prev_time)/1000000.0));
        sprintf(text, "Voltage = %d", (int)result);
        draw_message(0, 0, text); 
        draw_message(10, 20, text_time); 
        ssd1306_update();
        prev_time = to_us_since_boot(get_absolute_time());  
        /*
        gpio_put(16, 1);
        sleep_ms(1000); 
        ssd1306_clear();
        ssd1306_update();
        gpio_put(16, 0);
        sleep_ms(1000);
        */
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

void draw_letter(unsigned char x, unsigned char y, char letter) {
    int letter_index = (int)letter;
    int i; 
    int ii;
    unsigned char color; 
    for (i = x; i < x + 5; i++) {
        // Iterating through the collumns of the letter
        uint8_t collumn = ASCII[letter_index - 32][i - x];
        for (ii = y; ii < y + 8; ii++) {
            color = ((0b1 << (ii - y)) & (collumn)) >> (ii - y); 
            ssd1306_drawPixel(i, ii, color);
        }
    }
}

void draw_message(unsigned char x, unsigned char y, char *message) {
    char letter = 'o'; 
    int i = 0; 
    int length_message = 0; 
    while (letter != '\0') {
        length_message++;
        letter = message[length_message];
    }
    for (i = 0; i < length_message; i++) {
        draw_letter((x + 5*i), y, message[i]);
    }
}


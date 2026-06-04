#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/gpio.h"

int main()
{
    stdio_init_all();
    stdio_init_all();
    // Turn on the adc 
    adc_init();
    adc_gpio_init(26);
    adc_select_input(0); 
    while(!stdio_usb_connected());

    while (true) {
        // read the adc 
        uint16_t result = adc_read();
        //print the voltage
        printf("%f\n\f", (float)3.3*result/4095.0);
        sleep_ms(100);
    }
}

#include <stdio.h>
#include "pico/stdlib.h"


#define SCK_PIN 16
#define DT_PIN 17
#define SIGNAL 18
#define clock_time_us 10


void init_hx711(); 
int read_bits();


int main() {
    stdio_init_all();
    init_hx711(); 
    while(!stdio_usb_connected()); 
    while (true) {
        // Wait for user to send a n value
        int steps; 
        scanf("%d", &steps); 

        // Read N times 
        int i;
        int raw_data[steps];
        float filtered_data[steps];

        for (i = 0; i < steps; i++) {
            raw_data[i] = read_bits(); 
        }

        // Filter data using IIR (exponential moving average)
        float alpha = 0.2f; // Smoothing factor: 0 < alpha < 1
        filtered_data[0] = (float)raw_data[0];
        for (i = 1; i < steps; i++) {
            filtered_data[i] = alpha * (float)raw_data[i] + (1.0f - alpha) * filtered_data[i - 1];
        }

        // Print to user: original data, filtered data, and time
        for (i = 0; i < steps; i++) {
            printf("%d %d %5.2f\n", i * 1000, raw_data[i], filtered_data[i]);
        }

        sleep_ms(1000);
    }
}


// Inittialization functions for the Force sensor 
void init_hx711() {
    gpio_init(SCK_PIN); 
    gpio_set_dir(SCK_PIN, GPIO_OUT); 
    gpio_init(DT_PIN); 
    gpio_set_dir(DT_PIN, GPIO_IN); 
}

// Read the 24 bits from the force sensor 
int read_bits() {
    while (gpio_get(DT_PIN)) {
        tight_loop_contents(); 
    }

    unsigned int val = 0; 
    int i = 0; 
    for (i=0; i < 24; i++) {
        gpio_put(SCK_PIN, 1); 
        sleep_us(clock_time_us); 
        val = ((val << 1) | (gpio_get(DT_PIN) ? 1 : 0));
        gpio_put(SCK_PIN, 0); 
        sleep_us(clock_time_us); 
    }
    // 25th bit for gain 
    gpio_put(SCK_PIN, 1); 
    sleep_us(clock_time_us); 
    gpio_put(SCK_PIN, 0); 
    sleep_us(clock_time_us); 

    // sign-extend 24-bit two's complement to 32-bit signed int
    if (val & 0x800000) {
        val |= 0xFF000000;
    }
    return (int)val; 
}
//
#include <stdio.h>
#include "pico/stdlib.h"


#define SCK_PIN 16
#define DT_PIN 17
#define SIGNAL 18
#define clock_time_us 10


void init_hx711(); 
int read_bits();


int main()
{
    stdio_init_all();
    init_hx711(); 
    while(!stdio_usb_connected()); 
    while (true) {
        // testing the board 
        unsigned int force; 
        force = read_bits(); 
        printf("Returned value: %d\n", force); 
        // Wait for user to send a n value

        // Read N times 

        // Filter data 

        // Print to user orignial data, filtered data, and time in ms

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
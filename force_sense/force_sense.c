#include <stdio.h>
#include "pico/stdlib.h"


#define SCK_PIN
#define DT_PIN


void init_hx711(); 


int main()
{
    stdio_init_all();

    while (true) {
        // Wait for user to send a n value

        // Read N times 

        // Filter data 

        // Print to user orignial data, filtered data, and time in ms


        sleep_ms(1000);
    }
}


// Inittialization functions for the Force sensor 
void init_hx711() {
    GPIO_init(); 

}

// Read the 24 bits from the force sensor 

//
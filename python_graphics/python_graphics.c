#include <stdio.h>
#include "pico/stdlib.h"

#define BUTTON 16


int main()
{
    stdio_init_all();

    gpio_init(BUTTON); 
    gpio_set_dir(BUTTON, GPIO_IN);

    while (true) {
        printf("%d\n", gpio_get(BUTTON));
        sleep_ms(100);
    }
}

#include "HW2.h"

int main()
{
    stdio_init_all();
    while (true) {
        int i; 
        for (i=10; i<170; i++) {
            set_servo_postion(i);
            sleep_ms(20);
        }
        for (i=170; i>10; i--) {
            set_servo_postion(i);
            sleep_ms(20);
        }
    }
}

void init_servo_pwm() {
    // Initializing the Pulse with modulation of the servo motor
    gpio_set_function(SERVO, GPIO_FUNC_PWM); // Set the LED Pin to be PWM
    uint slice_num = pwm_gpio_to_slice_num(SERVO); // Get PWM slice number
    // the clock frequency is 150MHz divided by a float from 1 to 255
    float div = 50; // must be between 1-255
    pwm_set_clkdiv(slice_num, div); // sets the clock speed
    uint16_t wrap = 60000;
    // set the PWM frequency and resolution
    // this sets the PWM frequency to 150MHz / div / wrap
    pwm_set_wrap(slice_num, wrap);
    pwm_set_enabled(slice_num, true); // turn on the PWM
}

void set_servo_postion(int angle) {
    int duty;
    duty = (int)(0.05 + angle*0.05/180)*60000; 
    pwm_set_gpio_level(SERVO, duty);
}
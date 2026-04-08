#include "HW2.h"

int main()
{
    stdio_init_all();
    init_servo_pwm();
    while(!stdio_usb_connected());
    sleep_ms(100);
    printf("Usb connected\n");
    while (true) {
        int i;
        for (i=0; i<180; i++) {
            set_servo_postion(i);
            sleep_ms(10);
        }
        for (i=180; i>0; i--) {
            set_servo_postion(i);
            sleep_ms(10);
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

void set_servo_postion(float angle) { 
    pwm_set_gpio_level(SERVO, (int)((0.02 + angle*0.11/180.0)*60000));
}
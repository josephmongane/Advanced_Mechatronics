#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define SERVO 16

void init_servo_pwm(); 
void set_servo_postion(float angle); 

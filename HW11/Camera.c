#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include <string.h>
#include <stdlib.h>

#include "hardware/pwm.h"

#define BAUD_RATE 115200

#define BOARD_LED 25
#define motor_pwm 16

void my_pwm_init(void){

    gpio_set_function(motor_pwm, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(motor_pwm);
    float div = 40;
    pwm_set_clkdiv(slice_num, div);
    uint16_t wrap = 62500; // when to rollover, must be less than 65535
    pwm_set_wrap(slice_num, wrap);
    pwm_set_enabled(slice_num, true); // turn on the PWM

}

void set_motor_angle(int degree){

    float duty_cycle = 0.025 + ((float)degree / 180.0) * (0.125 - 0.025);
    uint16_t level = (uint16_t)(duty_cycle * 62500);

    pwm_set_gpio_level(motor_pwm, level);

}


void main(){
    stdio_init_all();

    // initializing the onboard LED
    gpio_init(BOARD_LED);
    gpio_set_dir(BOARD_LED, GPIO_OUT);

    my_pwm_init();

    bool first_time = true;
    
    while (1) {
        // Heartbeat led
        gpio_put(BOARD_LED, 1);
        sleep_ms(100);
        gpio_put(BOARD_LED, 0);
        sleep_ms(100);

        while(first_time){
            set_motor_angle(0);
            sleep_ms(100);
            int i=0;

            for(i;i<181;){
                set_motor_angle(i);
                sleep_ms(46);
                i = i + 4;
            }
            for(i;i>=0;i--){
                set_motor_angle(i);
                sleep_ms(46);
                i = i - 4;

            }
            first_time = false;
        }
        
    }
}




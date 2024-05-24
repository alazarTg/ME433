#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include <string.h>
#include <stdlib.h>

#include "hardware/pwm.h"

#define BAUD_RATE 115200

#define BOARD_LED 25
#define R_motor_pwm 16

#define L_motor_pwm 17

void my_pwm_init(void){

    gpio_set_function(R_motor_pwm, GPIO_FUNC_PWM);
    uint slice_num1 = pwm_gpio_to_slice_num(R_motor_pwm);
    float div = 62.5;
    pwm_set_clkdiv(slice_num1, div);
    uint16_t wrap = 100; // when to rollover, must be less than 65535
    pwm_set_wrap(slice_num1, wrap);
    pwm_set_enabled(slice_num1, true); // turn on the PWM

    gpio_set_function(L_motor_pwm, GPIO_FUNC_PWM);
    uint slice_num2 = pwm_gpio_to_slice_num(L_motor_pwm);
    pwm_set_clkdiv(slice_num2, div);
    pwm_set_wrap(slice_num2, wrap);
    pwm_set_enabled(slice_num2, true); // turn on the PWM

}

void set_motors_speed(int center_line){

    if (center_line == 0){

        pwm_set_gpio_level(R_motor_pwm, 100);
        pwm_set_gpio_level(L_motor_pwm, 0);

    }

    else if (center_line == 100){

        pwm_set_gpio_level(R_motor_pwm, 0);
        pwm_set_gpio_level(L_motor_pwm, 100);

    }

    else if (center_line < 40){

        pwm_set_gpio_level(R_motor_pwm, 100);
        pwm_set_gpio_level(L_motor_pwm, 80 + (10 * center_line)/20);

    }

    else if (center_line < 60){

        pwm_set_gpio_level(R_motor_pwm, 100);
        pwm_set_gpio_level(L_motor_pwm, 100);

    }

    else {

        pwm_set_gpio_level(R_motor_pwm, 100 - 200*1/(100 - center_line));;
        pwm_set_gpio_level(L_motor_pwm, 100);

    }
}

void main(){
    stdio_init_all();

    while (!stdio_usb_connected()) {
        sleep_ms(100);
    }

    // initializing the onboard LED
    gpio_init(BOARD_LED);
    gpio_set_dir(BOARD_LED, GPIO_OUT);

    my_pwm_init();

    // bool first_time = true;

    while (1) {
        // Heartbeat led
        gpio_put(BOARD_LED, 1);
        sleep_ms(100);
        gpio_put(BOARD_LED, 0);
        sleep_ms(100);

        printf("Enter a number to set PWM accordingly [0,100]: ");
        char message[100];
        scanf("%s", message);
        printf("\r\nYou entered: %s\r\n",message);

        set_motors_speed(atoi(message));
    }
}




#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include <string.h>
#include <stdlib.h>

#include "hardware/pwm.h"
#include "hardware/uart.h"
#include "hardware/irq.h"

#define UART_ID uart0
#define BAUD_RATE 115200
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY    UART_PARITY_NONE

#define UART_TX_PIN 0
#define UART_RX_PIN 1

static int chars_rxed = 0;

volatile int i = 0;
volatile char m[100];

#define BOARD_LED 25
#define R1_motor_pwm 17
#define R2_motor_pwm 16

#define L1_motor_pwm 14
#define L2_motor_pwm 15

// RX interrupt handler
void on_uart_rx() {
    while (uart_is_readable(UART_ID)) {
        uint8_t ch = uart_getc(UART_ID);

        if (ch == '\n'){
            m[i] = '\0';
            i = 0;
            set_motors_speed(atoi(m));
        }
        else {
            m[i] = ch;
            i++;
        }
        
        chars_rxed++;
    }
}

void setup_uart(){
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    // Actually, we want a different speed
    // The call will return the actual baud rate selected, which will be as close as
    // possible to that requested
    int __unused actual = uart_set_baudrate(UART_ID, BAUD_RATE);

    // Set UART flow control CTS/RTS, we don't want these, so turn them off
    uart_set_hw_flow(UART_ID, false, false);

    // Set our data format
    uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);

    // Turn off FIFO's - we want to do this character by character
    uart_set_fifo_enabled(UART_ID, false);

    // Set up a RX interrupt
    // We need to set up the handler first
    // Select correct interrupt for the UART we are using
    int UART_IRQ = UART_ID == uart0 ? UART0_IRQ : UART1_IRQ;

    // And set up and enable the interrupt handlers
    irq_set_exclusive_handler(UART_IRQ, on_uart_rx);
    irq_set_enabled(UART_IRQ, true);

    // Now enable the UART to send interrupts - RX only
    uart_set_irq_enables(UART_ID, true, false);
}

void my_pwm_init(void){

    gpio_set_function(R1_motor_pwm, GPIO_FUNC_PWM);
    gpio_set_function(R2_motor_pwm, GPIO_FUNC_PWM);
    gpio_set_function(L1_motor_pwm, GPIO_FUNC_PWM);
    gpio_set_function(L2_motor_pwm, GPIO_FUNC_PWM);


    uint slice_num1 = pwm_gpio_to_slice_num(L1_motor_pwm);
    uint slice_num2 = pwm_gpio_to_slice_num(L2_motor_pwm);
    uint slice_num3 = pwm_gpio_to_slice_num(R1_motor_pwm);
    uint slice_num4 = pwm_gpio_to_slice_num(R2_motor_pwm);

    float div = 62.5;


    pwm_set_clkdiv(slice_num1, div);
    pwm_set_clkdiv(slice_num2, div);
    pwm_set_clkdiv(slice_num3, div);
    pwm_set_clkdiv(slice_num4, div);

    uint16_t wrap = 100; // when to rollover, must be less than 65535


    pwm_set_wrap(slice_num1, wrap);
    pwm_set_wrap(slice_num2, wrap);
    pwm_set_wrap(slice_num3, wrap);
    pwm_set_wrap(slice_num4, wrap);

    pwm_set_enabled(slice_num1, true); // turn on the PWM
    pwm_set_enabled(slice_num2, true); // turn on the PWM
    pwm_set_enabled(slice_num3, true); // turn on the PWM
    pwm_set_enabled(slice_num4, true); // turn on the PWM
}

void set_motors_speed(int center_line){

    if (center_line == 0){

        pwm_set_gpio_level(R1_motor_pwm, 75);
        pwm_set_gpio_level(L1_motor_pwm, 0);

        pwm_set_gpio_level(R2_motor_pwm, 0);
        pwm_set_gpio_level(L2_motor_pwm, 0);

    }

    else if (center_line == 100){

        pwm_set_gpio_level(R1_motor_pwm, 0);
        pwm_set_gpio_level(L1_motor_pwm, 75);

        pwm_set_gpio_level(R2_motor_pwm, 0);
        pwm_set_gpio_level(L2_motor_pwm, 0);

    }

    else if (center_line < 45){

        // pwm_set_gpio_level(R1_motor_pwm, 95 - (10* center_line)/20);
        // pwm_set_gpio_level(R1_motor_pwm, 100 - 200*(100 - center_line));
        pwm_set_gpio_level(R1_motor_pwm, 75);
        pwm_set_gpio_level(L1_motor_pwm, 50 + (10*30)/20);

        pwm_set_gpio_level(R2_motor_pwm, 0);
        pwm_set_gpio_level(L2_motor_pwm, 0);

    }

    else if (center_line < 55){

        pwm_set_gpio_level(R1_motor_pwm, 75);
        pwm_set_gpio_level(L1_motor_pwm, 75);

        pwm_set_gpio_level(R2_motor_pwm, 0);
        pwm_set_gpio_level(L2_motor_pwm, 0);

    }

    else if (center_line < 100){

        pwm_set_gpio_level(R1_motor_pwm, 101 - (10 * center_line)/20);
        pwm_set_gpio_level(L1_motor_pwm, 75);
        // pwm_set_gpio_level(L1_motor_pwm, 0);

        pwm_set_gpio_level(R2_motor_pwm, 0);
        pwm_set_gpio_level(L2_motor_pwm, 0);    
    }
}

void main(){
    stdio_init_all();
    uart_init(UART_ID, 2400);

    // while (!stdio_usb_connected()) {
    //     sleep_ms(100);
    // }

    // initializing the onboard LED
    gpio_init(BOARD_LED);
    gpio_set_dir(BOARD_LED, GPIO_OUT);

    setup_uart();
    my_pwm_init();

    // bool first_time = true;

    while (1) {
        tight_loop_contents();
        // Heartbeat led
        gpio_put(BOARD_LED, 1);
        sleep_ms(100);
        gpio_put(BOARD_LED, 0);
        sleep_ms(100);

        // char message[100];
        // scanf("%s", message);
    }
}




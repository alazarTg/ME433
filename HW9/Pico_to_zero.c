#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include <string.h>
#include <stdlib.h>

#include "hardware/uart.h"
#include "hardware/irq.h"

#define UART_ID uart0
#define BAUD_RATE 115200
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY    UART_PARITY_NONE

#define BOARD_LED 25

// We are using pins 0 and 1, but see the GPIO function select table in the
// datasheet for information on which other pins can be used.
#define UART_TX_PIN 0
#define UART_RX_PIN 1

static int chars_rxed = 0;

volatile int i = 0;
volatile char m[100];

// RX interrupt handler
void on_uart_rx() {
    while (uart_is_readable(UART_ID)) {
        uint8_t ch = uart_getc(UART_ID);

        if (ch == '\n'){
            m[i] = '\0';
            printf("From ras-Pi: %s\r\n", m);
            i = 0;
        }
        else {
            m[i] = ch;
            i++;
        }
        
        chars_rxed++;
    }
}

void main(){
    stdio_init_all();
    uart_init(UART_ID, 2400);
    
    while (!stdio_usb_connected()) {
        sleep_ms(100);
    }

    // while (!stdio_usb_connected()) {
    //     sleep_ms(100);
    // }
    // printf("Start!\n");

    // Set the TX and RX pins by using the function select on the GPIO
    // Set datasheet for more information on function select
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

    // OK, all set up.

    // initializeing the onboard LED
    gpio_init(BOARD_LED);
    gpio_set_dir(BOARD_LED, GPIO_OUT);

    while (1) {
        tight_loop_contents();
        // Heartbeat led
        gpio_put(BOARD_LED, 1);
        sleep_ms(100);
        gpio_put(BOARD_LED, 0);
        sleep_ms(100);
        
        char message[100];
        scanf("%s", message);
        printf("Pico sent: %s\r\n",message);
        uart_puts(UART_ID, message);
    }
}



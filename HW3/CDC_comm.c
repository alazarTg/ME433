#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include <string.h>
#include <stdlib.h>

// The LED is connected to GPIO 25
#define LED_PIN 25
#define BUTTON 16

// Main (runs on core 0)
int main() {
    stdio_init_all();
    while (!stdio_usb_connected()) {
        sleep_ms(100);
    }

    printf("Start!\n");
    // Configure the LED pin
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    // Configure the Button pin
    gpio_init(BUTTON);
    gpio_set_dir(BUTTON, GPIO_IN);

    // adc module
    adc_init(); // init the adc module
    adc_gpio_init(26); // set ADC0 pin to be adc input instead of GPIO
    adc_select_input(0); // select to read from ADC0

    while (1) {
        // set LED high
        gpio_put(LED_PIN, 1);
        // wait for button press
        while(gpio_get(BUTTON) == 0){;}
        // button pressed so turn of LED
        gpio_put(LED_PIN, 0);
        printf("Enter a number of analog samples to take [1,100]: ");
        char message[100];
        scanf("%s", message);
        printf("\r\nYou entered: %s\r\n",message);
        int repeat_val;
        repeat_val = atoi(message);
        for(int i=0; i<repeat_val;i++){
            uint16_t result = adc_read();
            const float conversion_factor = 3.3f / (1 << 12);
            printf("%i\tvoltage: %f V\r\n", i+1, result * conversion_factor);
            sleep_ms(10);
        }

        sleep_ms(100);
    }
}
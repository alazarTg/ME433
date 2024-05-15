#include "font.h"
#include "hardware/adc.h"

void main(){
    stdio_init_all();
    while (!stdio_usb_connected()) {
        sleep_ms(100);
    }
    printf("Start!\n");
    
    // use I2C0 at 10kHz.
    i2c_init(i2c_default, 1000000);
    gpio_set_function(4, GPIO_FUNC_I2C);
    gpio_set_function(5, GPIO_FUNC_I2C);

    // initializeing the onboard LED
    gpio_init(BOARD_LED);
    gpio_set_dir(BOARD_LED, GPIO_OUT);

    adc_init();
    adc_gpio_init(26);
    adc_select_input(0);

    // Initializing the ssd1306 chip and setting the appropriate pins to outputs/inputs
    ssd1306_setup();

    const float conversion_factor = 3.3f / (1 << 12);

    while (1) {
        // Heartbeat led
        gpio_put(BOARD_LED, 1);
        sleep_ms(100);
        gpio_put(BOARD_LED, 0);
        sleep_ms(100);

        int x;
        int y;
        char message[50];
        unsigned int start = to_us_since_boot(get_absolute_time());
        uint16_t result = adc_read();
        ssd1306_clear();
        sprintf(message, "Val of ADC0 in V: %0.3f", result*conversion_factor);
        drawString(0, 0, message);
        unsigned int stop = to_us_since_boot(get_absolute_time());
        unsigned int t = stop - start;
        sprintf(message, "FPS = %f", 100000.0/t);
        drawString(0,24, message);
        ssd1306_update();
    }
}



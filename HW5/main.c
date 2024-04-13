#include "I2C_communication.h"

void main(){
    stdio_init_all();
    
    // use I2C0 at 10kHz.
    i2c_init(i2c_default, 10000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);

    // initializeing the onboard LED
    gpio_init(BOARD_LED);
    gpio_set_dir(BOARD_LED, GPIO_OUT);

    // Initializing the MCP_23008 chip and setting the appropriate pins to outputs/inputs
    MCP_23008_init();

    while (1) {
        // Heartbeat led
        gpio_put(BOARD_LED, 1);
        sleep_ms(100);
        gpio_put(BOARD_LED, 0);
        sleep_ms(100);

        // iF button is pressed, turn on led
        if(MCP_23008_button_pressed()){
            MCP_23008_LED_on();
        }
        // if button is not pressed, turn off led
        else{
            MCP_23008_LED_off();
        }
    }

}



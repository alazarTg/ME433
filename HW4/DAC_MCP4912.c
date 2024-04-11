#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/adc.h"
#include "hardware/spi.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define LED_PIN 1
#define M_PI acos(-1.0)

#ifdef PICO_DEFAULT_SPI_CSN_PIN
static inline void cs_select() {
    asm volatile("nop \n nop \n nop");
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 0);  // Active low
    asm volatile("nop \n nop \n nop");
}

static inline void cs_deselect() {
    asm volatile("nop \n nop \n nop");
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 1);
    asm volatile("nop \n nop \n nop");
}
#endif

#if defined(spi_default) && defined(PICO_DEFAULT_SPI_CSN_PIN)
static void write_register(uint8_t reg, uint8_t data) {
    uint8_t buf[2];
    buf[0] = reg & 0x7f;  // remove read bit as this is a write
    buf[1] = data;
    cs_select();
    spi_write_blocking(spi_default, buf, 2);
    cs_deselect();
}
#endif

// Given an the address of an array, the function creates a sin wave pattern using values from 0-1023. One sin wave
// uses 100 data points
void make_sin_array(int16_t* sin_numbers){

    for(int i = 0; i < 100; i++){
        double angle = i * 2 * M_PI/100;

        sin_numbers[i] = (int)((sin(angle) + 1)*511.5);
    }
    
}

// Given an the address of an array, the function creates a triangle wave pattern using values from 0-1023. One triangle wave
// uses 100 data points
void make_triangle_array(int16_t* triangle_numbers){

    double slope = 1023/50.5;
    for(int i = 0; i < 51; i++){

        triangle_numbers[i] = (int)(i * slope);
        triangle_numbers[100-i] = (int)(i * slope);
    }
}

// Given a sixteen bit integer between 0-1023, the function parses the bits and creates the appropriate two eight bit integers
// to call the write_register function
void int_to_bits(int16_t ten_bit_number){

    uint8_t passed_bits[2];
    uint8_t register_bits;
    uint8_t data_bits[2];

    register_bits = 0b00110000;
    passed_bits[0] = register_bits + (ten_bit_number >> 6);
    passed_bits[1] = (uint8_t)ten_bit_number << 2;

    write_register(passed_bits[0], passed_bits[1]);
}


void main() {

    // use SPI0 at 2MHz.
    spi_init(spi_default, 2000000);
    // gpio_set_function(PICO_DEFAULT_SPI_RX_PIN, GPIO_FUNC_SPI); #Not using the RX pin
    gpio_set_function(PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI);

    // Chip select is active-low, so we'll initialise it to a driven-high state
    gpio_init(PICO_DEFAULT_SPI_CSN_PIN);
    gpio_set_dir(PICO_DEFAULT_SPI_CSN_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 1);

    // create an array of 100 values that range from 0-1023 and make a sin wave output
    int16_t my_sin_array[100];
    make_sin_array(my_sin_array);

    // create an array of 100 values that range form 0-1023 and make a triangle wave output
    int16_t my_triangle_array[101];
    make_triangle_array(my_triangle_array);

    while (1) {
        // Generate a 2Hz sin wave
        for(int x=0; x<100;x++){
            int_to_bits(my_sin_array[x]);
            sleep_ms(5);
        }
        // // Generate a 1Hz sin wave
        // for(int x=0; x<101;x++){
        //     int_to_bits(my_triangle_array[x]);
        //     sleep_ms(10);
        // }
    }
}
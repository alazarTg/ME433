#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"
#include <string.h>
#include <stdlib.h>

// Address of chip used for I2C communication
#define ADDR _u(0b0100000)

// hardware registers
#define REG_IODIR _u(0x00)
#define REG_GPIO _u(0x09)
#define REG_OLAT _u(0x0A)

#define BOARD_LED 25

// Initialzies the chip and sets the approriate pins to inputs and outputs
void MCP_23008_init();

// Turns on the led that is on GP7
void MCP_23008_LED_on();

// Turns off the led that is on GP7
void MCP_23008_LED_off();

// Returns a bool TRUE if the button on GP0 is pushed and FALSE if it's not
bool MCP_23008_button_pressed();



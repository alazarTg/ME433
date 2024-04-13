#include "I2C_communication.h"

void MCP_23008_init(){
    uint8_t buf[2];

    // send register number followed by its corresponding value
    buf[0] = REG_IODIR;
    buf[1] = 0b01111111;
    i2c_write_blocking(i2c_default, ADDR, buf, 2, false);
}

void MCP_23008_LED_on(){

    uint8_t buf[2];

    // send register number followed by its corresponding value
    buf[0] = REG_OLAT;
    buf[1] = 0b1 << 7;
    i2c_write_blocking(i2c_default, ADDR, buf, 2, false);
}

void MCP_23008_LED_off(){
    uint8_t buf[2];

    // send register number followed by its corresponding value
    buf[0] = REG_OLAT;
    buf[1] = 0b0 << 7;
    i2c_write_blocking(i2c_default, ADDR, buf, 2, false);
}

bool MCP_23008_button_pressed() {
    
    uint8_t buf[1];
    uint8_t reg = REG_GPIO;
    i2c_write_blocking(i2c_default, ADDR, &reg, 1, true);  // true to keep master control of bus
    i2c_read_blocking(i2c_default, ADDR, buf, 1, false);  // false - finished with bus

    // Button is pulled high so goes low when pressed
    if((buf[0] & 0b1) == 0){
        return true; 
    }
    return false;
}
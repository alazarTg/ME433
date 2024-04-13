add_executable(main I2C_communication main.c I2C_communication.c I2C_communication.h)


# Pull in our pico_stdlib which pulls in commonly used features
target_link_libraries(main pico_stdlib hardware_i2c I2C_communication)

pico_enable_stdio_usb(main 1)
pico_enable_stdio_uart(main 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(main)


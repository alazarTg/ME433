add_executable(main main.c I2C_communication.c)


# Pull in our pico_stdlib which pulls in commonly used features
target_link_libraries(main pico_stdlib hardware_i2c)

pico_enable_stdio_usb(main 1)
pico_enable_stdio_uart(main 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(main)


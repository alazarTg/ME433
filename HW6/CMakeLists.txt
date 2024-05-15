add_executable(main_display main_display.c ssd1306.c font.c)


# Pull in our pico_stdlib which pulls in commonly used features
target_link_libraries(main_display pico_stdlib hardware_i2c hardware_adc)

pico_enable_stdio_usb(main_display 1)
pico_enable_stdio_uart(main_display 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(main_display)


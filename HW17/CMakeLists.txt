add_executable(Pico_final Pico_final.c)


# Pull in our pico_stdlib which pulls in commonly used features
target_link_libraries(Pico_final pico_stdlib hardware_pwm hardware_uart)

pico_enable_stdio_usb(Pico_final 1)
pico_enable_stdio_uart(Pico_final 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(Pico_final)


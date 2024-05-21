add_executable(Pico_to_zero Pico_to_zero.c)


# Pull in our pico_stdlib which pulls in commonly used features
target_link_libraries(Pico_to_zero pico_stdlib hardware_uart)

pico_enable_stdio_usb(Pico_to_zero 1)
pico_enable_stdio_uart(Pico_to_zero 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(Pico_to_zero)


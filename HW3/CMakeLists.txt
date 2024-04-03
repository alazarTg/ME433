add_executable(CDC_comm CDC_comm.c)


# Pull in our pico_stdlib which pulls in commonly used features
target_link_libraries(CDC_comm pico_stdlib hardware_adc)

pico_enable_stdio_usb(CDC_comm 1)
pico_enable_stdio_uart(CDC_comm 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(CDC_comm)


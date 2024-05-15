add_executable(UART_comm UART_comm.c font.c ssd1306.c)


# Pull in our pico_stdlib which pulls in commonly used features
target_link_libraries(UART_comm pico_stdlib hardware_i2c hardware_uart)

pico_enable_stdio_usb(UART_comm 1)
pico_enable_stdio_uart(UART_comm 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(UART_comm)


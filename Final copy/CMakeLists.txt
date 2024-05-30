add_executable(Final_attempt Final_attempt.c)


# Pull in our pico_stdlib which pulls in commonly used features
target_link_libraries(Final_attempt pico_stdlib hardware_pwm hardware_uart)

pico_enable_stdio_usb(Final_attempt 1)
pico_enable_stdio_uart(Final_attempt 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(Final_attempt)


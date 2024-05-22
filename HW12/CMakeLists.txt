add_executable(Servo_control Servo_control.c)


# Pull in our pico_stdlib which pulls in commonly used features
target_link_libraries(Servo_control pico_stdlib hardware_pwm)

pico_enable_stdio_usb(Servo_control 1)
pico_enable_stdio_uart(Servo_control 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(Servo_control)


add_executable(Motor_controller Motor_controller.c)


# Pull in our pico_stdlib which pulls in commonly used features
target_link_libraries(Motor_controller pico_stdlib hardware_pwm)

pico_enable_stdio_usb(Motor_controller 1)
pico_enable_stdio_uart(Motor_controller 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(Motor_controller)


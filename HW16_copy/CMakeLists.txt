add_executable(Motor_controller_copy Motor_controller_copy.c)


# Pull in our pico_stdlib which pulls in commonly used features
target_link_libraries(Motor_controller_copy pico_stdlib hardware_pwm)

pico_enable_stdio_usb(Motor_controller_copy 1)
pico_enable_stdio_uart(Motor_controller_copy 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(Motor_controller_copy)


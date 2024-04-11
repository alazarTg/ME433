add_executable(DAC_MCP4912 DAC_MCP4912.c)


# Pull in our pico_stdlib which pulls in commonly used features
target_link_libraries(DAC_MCP4912 pico_stdlib hardware_adc hardware_spi)

pico_enable_stdio_usb(DAC_MCP4912 1)
pico_enable_stdio_uart(DAC_MCP4912 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(DAC_MCP4912)


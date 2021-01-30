cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)

project(picoprobe)

pico_sdk_init()

add_executable(picolamp
        picolamp.c
)

# Pull in our pico_stdlib which aggregates commonly used features
target_link_libraries(picolamp pico_stdlib hardware_pwm)

# enable usb output, disable uart output
pico_enable_stdio_usb(picolamp 1)
pico_enable_stdio_uart(picolamp 0)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(picolamp)
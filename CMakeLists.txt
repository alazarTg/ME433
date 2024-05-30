cmake_minimum_required(VERSION 3.12)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

project(ME433 C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)


set(ME433_PATH ${PROJECT_SOURCE_DIR})

# Initialize the SDK
pico_sdk_init()

add_subdirectory(HW2)
add_subdirectory(HW3)
add_subdirectory(HW4)
add_subdirectory(HW5)
add_subdirectory(HW6)
add_subdirectory(HW7)

add_subdirectory(HW9)


add_subdirectory(HW12)



add_subdirectory(HW16)
add_subdirectory(HW16_copy)

add_subdirectory(HW17)


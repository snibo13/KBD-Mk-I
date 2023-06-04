cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(Jeyboard C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(Keyboard main.cpp usb_descriptors.cpp)
target_include_directories(Keyboard PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/ )
target_link_libraries(Keyboard PRIVATE pico_stdlib tinyusb_device tinyusb_board)
pico_add_extra_outputs(Keyboard)
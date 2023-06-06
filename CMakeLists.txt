cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(Keyboard C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(Keyboard main.cpp ${CMAKE_CURRENT_SOURCE_DIR}/lib/TFT_eSPI/TFT_eSPI.cpp usb_descriptors.c hid.cpp)
target_include_directories(Keyboard PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/ ${CMAKE_CURRENT_SOURCE_DIR}/lib/lvgl ${CMAKE_CURRENT_SOURCE_DIR}/lib/TFT_eSPI ${CMAKE_CURRENT_SOURCE_DIR}/lib)
target_link_libraries(Keyboard PRIVATE pico_stdlib tinyusb_device tinyusb_board hardware_spi )
pico_add_extra_outputs(Keyboard)
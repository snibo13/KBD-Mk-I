cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)
add_subdirectory(lvgl)

project(Keyboard C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(Keyboard 
main.cpp 
hid.cpp
keyboard.cpp 
lcd.cpp 
usb_descriptors.c 
shift_register.cpp
interface.cpp)
target_include_directories(Keyboard PRIVATE 
${CMAKE_CURRENT_SOURCE_DIR}/ 
${CMAKE_CURRENT_SOURCE_DIR}/lvgl/
${CMAKE_CURRENT_SOURCE_DIR}/messages/
/home/sidney/Documents/Code/utils/nanopb-0.4.7-linux-x86/
)

target_link_libraries(Keyboard PRIVATE 
pico_stdlib 
tinyusb_device
tinyusb_board 
hardware_spi  
lvgl
)
# enable usb output, disable uart output
pico_enable_stdio_usb(Keyboard 1)
# pico_enable_stdio_uart(Keyboard 0)
pico_add_extra_outputs(Keyboard)
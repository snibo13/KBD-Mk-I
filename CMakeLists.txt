cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)
add_subdirectory(lvgl)

project(Keyboard C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(Keyboard main.cpp  usb_descriptors.c hid.cpp disp.cpp)
target_include_directories(Keyboard PRIVATE 
${CMAKE_CURRENT_SOURCE_DIR}/ 
${CMAKE_CURRENT_SOURCE_DIR}/lvgl/
# ${CMAKE_CURRENT_SOURCE_DIR}/lvgl/src/lv_core
# ${CMAKE_CURRENT_SOURCE_DIR}/lvgl/src/lv_hal
# ${CMAKE_CURRENT_SOURCE_DIR}/lvgl/src/lv_objx
# ${CMAKE_CURRENT_SOURCE_DIR}/lvgl/src/lv_font
# ${CMAKE_CURRENT_SOURCE_DIR}/lvgl/src/lv_misc
# ${CMAKE_CURRENT_SOURCE_DIR}/lvgl/src/lv_themes
# ${CMAKE_CURRENT_SOURCE_DIR}/lvgl/src/lv_draw

)
target_link_libraries(Keyboard PRIVATE 
pico_stdlib 
tinyusb_device
tinyusb_board 
hardware_spi 
lvgl
# ${CMAKE_CURRENT_SOURCE_DIR}/lvgl/
# ${CMAKE_CURRENT_SOURCE_DIR}/lvgl/build/lib/
# ${CMAKE_CURRENT_SOURCE_DIR}/lvgl/src/lv_core
# ${CMAKE_CURRENT_SOURCE_DIR}/lvgl/src/lv_hal
# ${CMAKE_CURRENT_SOURCE_DIR}/lvgl/src/lv_objx
# ${CMAKE_CURRENT_SOURCE_DIR}/lvgl/src/lv_font
# ${CMAKE_CURRENT_SOURCE_DIR}/lvgl/src/lv_misc
# ${CMAKE_CURRENT_SOURCE_DIR}/lvgl/src/lv_themes
# ${CMAKE_CURRENT_SOURCE_DIR}/lvgl/src/lv_draw
)
pico_add_extra_outputs(Keyboard)
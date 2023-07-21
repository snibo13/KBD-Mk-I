#include <stdlib.h>
#include <stdio.h>
#include <pico/stdlib.h>
#include <pico/stdio.h>
#include "hardware/spi.h"
#include "hardware/uart.h"

#include "bsp/board.h"

#include "usb_descriptors.h"
#include "tusb.h"

#include "lv_conf.h"
#include "lvgl.h"

#include "lcd.h"
#include "shift_register.h"

#include "keyboard.h"
#include "hid.h"

static int profile_number = 3;
int main(void)
{
    board_init();
    stdio_init_all();
    tusb_init();

    // Initialize the LCD
    // lcd_init();
    // lv_init();
    // hal_setup();

    // lv_obj_set_style_bg_color(lv_scr_act(), lv_color_make(0, 255, 0), LV_PART_MAIN);

    // lv_obj_t *reg_label = lv_label_create(lv_scr_act());
    // lv_obj_align(reg_label, LV_ALIGN_CENTER, 0, 0);

    initialise_register();

    while (true)
    {
        // printf("Loop\n");
        tud_task();
        led_blinking_task();

        keyboard_task();
    }
    return 0;
}
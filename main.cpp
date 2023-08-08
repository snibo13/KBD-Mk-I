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

#include "interface.h"

static int profile_number = 3;

void lv_example_get_started_1(void)
{
    /*Change the active screen's background color*/
    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0x003a57), LV_PART_MAIN);

    /*Create a white label, set its text and align it to the center*/
    lv_obj_t *label = lv_label_create(lv_scr_act());
    lv_label_set_text(label, interface_string);
    lv_obj_set_style_text_color(lv_scr_act(), lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
}

int main(void)
{
    board_init();
    stdio_init_all();
    tusb_init();

    // Initialize the LCD
    lcd_init();
    lv_init();
    hal_setup();

    lv_example_get_started_1();
    initialise_register();

    while (true)
    {
        tud_task();
        led_blinking_task();

        keyboard_task();
        interface_task();
        // hid_task();
        // sleep_ms(500);
        // lv_label_set_text(reg_label, bin_reg);
        lv_task_handler();
    }
    return 0;
}
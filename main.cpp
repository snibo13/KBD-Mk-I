// #include "usb.h"
#include <stdlib.h>
#include <stdio.h>
#include <pico/stdlib.h>
#include <pico/stdio.h>
#include "hardware/spi.h"

#include "bsp/board.h"
// #include "tusb.h"
// #include "usb_descriptors.h"

#include "hid.h"
#include "universe.h"

#include "lv_conf.h"
#include "lvgl.h"

#include "lcd.h"

// Function prototypes

uint16_t color565(uint8_t r, uint8_t g, uint8_t b)
{
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

int main(void)
{
    board_init();
    // tusb_init();

    init_lcd_hardware();
    // Initialize the LCD
    lcd_init();
    lcd_fill_screen(0); // Clear screen to black
    lv_init();
    hal_setup();

    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_make(0, 255, 0), LV_PART_MAIN);

    lv_obj_t *label = lv_label_create(lv_scr_act());
    lv_label_set_text(label, "Test 1");
    lv_obj_set_style_text_color(lv_scr_act(), lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    while (1)
    {

        // tud_task();
        // led_task();
        // hid_task();
        lv_task_handler();
        // tight_loop_contents();
    }
    return 0;
}

// #include "usb.h"
#include <stdlib.h>
#include <stdio.h>
#include <pico/stdlib.h>
#include <pico/stdio.h>
#include "hardware/spi.h"

#include "bsp/board.h"
#include "tusb.h"
#include "usb_descriptors.h"

#include "hid.h"
#include "universe.h"

#include "lv_conf.h"
#include "lvgl.h"

#include "lcd.h"

// Function prototypes
static uint32_t blink_interval_ms = BLINK_UNMOUNTED;

uint16_t color565(uint8_t r, uint8_t g, uint8_t b)
{
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

void led_task();
int main(void)
{
    board_init();
    tusb_init();

    init_lcd_hardware();
    // Initialize the LCD
    lcd_init();
    lcd_fill_screen(0); // Clear screen to black
    lv_init();
    hal_setup();

    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_make(0, 255, 0), LV_PART_MAIN);

    lv_obj_t *label = lv_label_create(lv_scr_act());
    lv_label_set_text(label, "Test 2");
    lv_obj_set_style_text_color(lv_scr_act(), lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    while (1)
    {

        // tud_task();
        led_task();
        // hid_task();
        lv_task_handler();
        // tight_loop_contents();
    }
    return 0;
}

// Device callbacks
void tud_mount_cb(void)
{
    blink_interval_ms = BLINK_MOUNTED;
}

void tud_unmount_cb(void)
{
    blink_interval_ms = BLINK_UNMOUNTED;
}

void tud_suspend_cb(bool remote_wakeup_en)
{
    (void)remote_wakeup_en;
    blink_interval_ms = BLINK_SUSPENDED;
}

void tud_resume_cb(void)
{
    blink_interval_ms = BLINK_MOUNTED;
}

// Debugging
void led_task(void)
{
    static uint32_t start_ms = 0;
    static bool led_state = false;

    if (!blink_interval_ms)
        return;

    // Blink every interval ms
    if (board_millis() - start_ms < blink_interval_ms)
        return; // not enough time
    start_ms += blink_interval_ms;

    board_led_write(led_state);
    led_state = 1 - led_state; // toggle
}
// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const *buffer, uint16_t bufsize)
{
    (void)instance;

    if (report_type == HID_REPORT_TYPE_OUTPUT)
    {
        // Set keyboard LED e.g Capslock, Numlock etc...
        if (report_id == REPORT_ID_KEYBOARD)
        {
            // bufsize should be (at least) 1
            if (bufsize < 1)
                return;

            uint8_t const kbd_leds = buffer[0];

            if (kbd_leds & KEYBOARD_LED_CAPSLOCK)
            {
                // Capslock On: disable blink, turn led on
                blink_interval_ms = 0;
                board_led_write(true);
            }
            else
            {
                // Caplocks Off: back to normal blink
                board_led_write(false);
                blink_interval_ms = BLINK_MOUNTED;
            }
        }
    }
}

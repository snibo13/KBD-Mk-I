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
// #include "lcd.h"

static uint32_t blink_interval_ms = BLINK_UNMOUNTED;

void led_task(void);
void hal_setup(void);
void lcd_init();
void lcd_command(uint8_t cmd);

// LCD pin definitions
#define PIN_LCD_CS 6
#define PIN_LCD_RST 4
#define PIN_LCD_DC 5
#define PIN_LCD_SCK 2
#define PIN_LCD_MOSI 3

// LCD commands
#define CMD_SWRESET 0x01
#define CMD_SLPOUT 0x11
#define CMD_DISPON 0x29
#define CMD_CASET 0x2A
#define CMD_RASET 0x2B
#define CMD_RAMWR 0x2C

// LCD dimensions
#define LCD_WIDTH 128
#define LCD_HEIGHT 180

int main(void)
{
    board_init();
    tusb_init();

    board_led_off();
    // Initialize the LCD
    lcd_init();
    // lcd_setup();

    // Initialize LVGL
    // lv_init();

    // lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0xff0000), LV_PART_MAIN);
    // lcd_fill_screen((uint16_t)0xffffff);
    // lcd_fill_screen(0);

    while (1)
    {
        tud_task();
        led_task();
        hid_task();
        // lv_task_handler();
        // lcd_fill_screen(0);
    }
    return 0;
}

void hal_setup(void)
{
    // static lv_disp_draw_buf_t disp_buf;
    // static lv_color_t buf[LCD_WIDTH * LCD_HEIGHT];
    // lv_disp_draw_buf_init(&disp_buf, buf, NULL, LCD_WIDTH * LCD_HEIGHT);

    // static lv_disp_drv_t disp_drv;
    // lv_disp_drv_init(&disp_drv);
    // disp_drv.flush_cb = lcd_flush;
    // disp_drv.hor_res = LCD_WIDTH;
    // disp_drv.ver_res = LCD_HEIGHT;

    // disp_drv.draw_buf = &disp_buf;

    // static lv_disp_t *disp;
    // disp = lv_disp_drv_register(&disp_drv);
    // lv_disp_set_default(disp);
}

// LCD Stuff
void lcd_init()
{
    lcd_command(CMD_SWRESET);
    sleep_ms(150);
    lcd_command(CMD_SLPOUT);
    sleep_ms(150);
    lcd_command(CMD_DISPON);
    sleep_ms(150);
}

void lcd_command(uint8_t cmd)
{
    gpio_put(PIN_LCD_CS, 1);           // Start listening
    gpio_put(PIN_LCD_DC, 1);           // Enter command mode
    spi_write_blocking(spi0, &cmd, 1); // Send command
    gpio_put(PIN_LCD_CS, 0);           // Stop listening
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
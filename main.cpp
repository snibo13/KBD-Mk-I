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

// #define ARDUINO_ARCH_RP2040
// #include "macros.h"
#include "lv_conf.h"
#include "lvgl.h"
// #include "lvgl/src/lv_conf_internal.h"
// #include "lvgl/src/core/lv_obj.h"

static uint32_t blink_interval_ms = BLINK_UNMOUNTED;

void led_task(void);

// LCD pin definitions
#define PIN_LCD_CS 5
#define PIN_LCD_RST 6
#define PIN_LCD_DC 7
#define PIN_LCD_SCK 8
#define PIN_LCD_MOSI 10

// LCD commands
#define CMD_SWRESET 0x01
#define CMD_SLPOUT 0x11
#define CMD_DISPON 0x29
#define CMD_CASET 0x2A
#define CMD_RASET 0x2B
#define CMD_RAMWR 0x2C

// LCD dimensions
#define LCD_WIDTH 128
#define LCD_HEIGHT 160

// Function prototypes
void lcd_init();
void lcd_command(uint8_t cmd);
void lcd_data(uint8_t data);
void lcd_set_address(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
void lcd_set_pixel(uint8_t x, uint8_t y, uint16_t color);
void lcd_fill_screen(uint16_t color);

int main(void)
{
    board_init();
    tusb_init();

    // Initialize SPI interface
    spi_init(spi0, 500000);
    gpio_set_function(PIN_LCD_SCK, GPIO_FUNC_SPI);
    gpio_set_function(PIN_LCD_MOSI, GPIO_FUNC_SPI);

    // Configure LCD pin directions
    gpio_init(PIN_LCD_CS);
    gpio_set_dir(PIN_LCD_CS, GPIO_OUT);
    gpio_init(PIN_LCD_RST);
    gpio_set_dir(PIN_LCD_RST, GPIO_OUT);
    gpio_init(PIN_LCD_DC);
    gpio_set_dir(PIN_LCD_DC, GPIO_OUT);

    // Reset the LCD
    gpio_put(PIN_LCD_RST, 0);
    sleep_ms(100);
    gpio_put(PIN_LCD_RST, 1);
    sleep_ms(100);

    // Initialize the LCD
    lcd_init();

    // Clear the screen
    lcd_fill_screen(0xFFFF);

    // Draw some pixels
    lcd_set_pixel(10, 10, 0x001F);
    lcd_set_pixel(20, 20, 0xF800);
    lcd_set_pixel(30, 30, 0x07E0);
    lcd_set_pixel(40, 40, 0xFFE0);

    lv_init();

    while (1)
    {
        tud_task();
        // led_task();
        hid_task();
        tight_loop_contents();
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

void lcd_init()
{
    // Send initialization commands to the LCD
    lcd_command(CMD_SWRESET); // Software reset
    sleep_ms(150);
    lcd_command(CMD_SLPOUT); // Sleep out
    sleep_ms(150);
    lcd_command(CMD_DISPON); // Display on
    sleep_ms(150);
}

void lcd_command(uint8_t cmd)
{
    gpio_put(PIN_LCD_DC, 0); // Set DC low for command mode
    spi_write_blocking(spi0, &cmd, 1);
}

void lcd_data(uint8_t data)
{
    gpio_put(PIN_LCD_DC, 1); // Set DC high for data mode
    spi_write_blocking(spi0, &data, 1);
}

void lcd_set_address(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{
    lcd_command(CMD_CASET); // Column address set
    lcd_data(0x00);
    lcd_data(x0);
    lcd_data(0x00);
    lcd_data(x1);

    lcd_command(CMD_RASET); // Row address set
    lcd_data(0x00);
    lcd_data(y0);
    lcd_data(0x00);
    lcd_data(y1);

    lcd_command(CMD_RAMWR); // Memory write
}

void lcd_set_pixel(uint8_t x, uint8_t y, uint16_t color)
{
    lcd_set_address(x, y, x, y);
    lcd_data(color >> 8);
    lcd_data(color);
}

void lcd_fill_screen(uint16_t color)
{
    lcd_set_address(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
    for (int i = 0; i < (LCD_WIDTH * LCD_HEIGHT); i++)
    {
        lcd_data(color >> 8);
        lcd_data(color);
    }
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
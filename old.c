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
#define PIN_LCD_RST 17
#define PIN_LCD_DC 6
#define PIN_LCD_SCK 18
#define PIN_LCD_MOSI 19

#define CS 5
#define CLK 18
#define MOSI 19
#define MISO 16
#define DC 6
#define RST 17

// LCD commands
#define CMD_SWRESET 0x01
#define CMD_SLPOUT 0x11
#define CMD_DISPON 0x29
#define CMD_CASET 0x2A
#define CMD_RASET 0x2B
#define CMD_RAMWR 0x2C
#define ST7735_COLOR_MODE 0x3AU /* Interface pixel format: COLMOD              */

// LCD dimensions
#define LCD_WIDTH 128
#define LCD_HEIGHT 160

// lv_disp_buf_t disp_buf;
// lv_color_t color_p[LCD_WIDTH * 10];

// Function prototypes
void lcd_init();
void lcd_command(uint8_t cmd);
void lcd_data(uint8_t data);
void lcd_set_address(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
void lcd_set_pixel(uint8_t x, uint8_t y, uint16_t color);
void lcd_fill_screen(uint16_t color);

void lcd_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *buf);

// Colours for convenience
#define ST7735_BLACK 0x0000   // 0b 00000 000000 00000
#define ST7735_BLUE 0x001F    // 0b 00000 000000 11111
#define ST7735_GREEN 0x07E0   // 0b 00000 111111 00000
#define ST7735_RED 0xF800     // 0b 11111 000000 00000
#define ST7735_CYAN 0x07FF    // 0b 00000 111111 11111
#define ST7735_MAGENTA 0xF81F // 0b 11111 000000 11111
#define ST7735_YELLOW 0xFFE0  // 0b 11111 111111 00000
#define ST7735_WHITE 0xFFFF   // 0b 11111 111111 11111

uint16_t color565(uint8_t r, uint8_t g, uint8_t b)
{
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

void hal_setup(void)
{
    static lv_disp_draw_buf_t disp_buf;
    static lv_color_t buf[LCD_WIDTH * LCD_HEIGHT];
    lv_disp_draw_buf_init(&disp_buf, buf, NULL, LCD_WIDTH * LCD_HEIGHT);

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.flush_cb = lcd_flush;
    disp_drv.hor_res = LCD_WIDTH;
    disp_drv.ver_res = LCD_HEIGHT;

    disp_drv.draw_buf = &disp_buf;

    static lv_disp_t *disp;
    disp = lv_disp_drv_register(&disp_drv);
    lv_disp_set_default(disp);
}

int main(void)
{
    board_init();
    // tusb_init();

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
    lcd_fill_screen(0); // Clear screen to white
    lv_init();
    hal_setup();

    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_make(255, 0, 0), LV_PART_MAIN);

    lv_obj_t *label = lv_label_create(lv_scr_act());
    lv_label_set_text(label, "Hello world");
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

// // Device callbacks
// void tud_mount_cb(void)
// {
//     blink_interval_ms = BLINK_MOUNTED;
// }

// void tud_unmount_cb(void
// {
//     blink_interval_ms = BLINK_UNMOUNTED;
// }

// void tud_suspend_cb(bool remote_wakeup_en)
// {
//     (void)remote_wakeup_en;
//     blink_interval_ms = BLINK_SUSPENDED;
// }

// void tud_resume_cb(void)
// {
//     blink_interval_ms = BLINK_MOUNTED;
// }

void lcd_init()
{
    // Send initialization commands to the LCD
    lcd_command(CMD_SWRESET); // Software reset
    sleep_ms(150);
    lcd_command(CMD_SLPOUT); // Sleep out
    sleep_ms(150);
    lcd_command(CMD_DISPON); // Display on
    sleep_ms(150);
    lcd_command(ST7735_COLOR_MODE);
    lcd_data(0x05);
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

void lcd_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *buf)
{
    /*The most simple case (but also the slowest) to put all pixels to the screen one-by-one
     *`put_px` is just an example, it needs to be implemented by you.*/
    int32_t x, y;
    uint16_t color;
    lcd_set_address(area->x1, area->y1, area->x2, area->y2);
    for (y = area->y1; y <= area->y2; y++)
    {
        for (x = area->x1; x <= area->x2; x++)
        {
            color = lv_color_to16(*buf);
            lcd_data(color >> 8);
            lcd_data(color);
            buf++;
        }
    }

    /* IMPORTANT!!!
     * Inform LVGL that you are ready with the flushing and buf is not used anymore*/
    lv_disp_flush_ready(disp);
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
// // Invoked when received SET_REPORT control request or
// // received data on OUT endpoint ( Report ID = 0, Type = 0 )
// void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const *buffer, uint16_t bufsize)
// {
//     (void)instance;

//     if (report_type == HID_REPORT_TYPE_OUTPUT)
//     {
//         // Set keyboard LED e.g Capslock, Numlock etc...
//         if (report_id == REPORT_ID_KEYBOARD)
//         {
//             // bufsize should be (at least) 1
//             if (bufsize < 1)
//                 return;

//             uint8_t const kbd_leds = buffer[0];

//             if (kbd_leds & KEYBOARD_LED_CAPSLOCK)
//             {
//                 // Capslock On: disable blink, turn led on
//                 blink_interval_ms = 0;
//                 board_led_write(true);
//             }
//             else
//             {
//                 // Caplocks Off: back to normal blink
//                 board_led_write(false);
//                 blink_interval_ms = BLINK_MOUNTED;
//             }
//         }
//     }
// }
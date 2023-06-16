#pragma once

#include "hardware/spi.h"
#include <pico/stdlib.h>
#include "lvgl.h"
#include "lv_conf.h"
// LCD pin definitions
#define PIN_LCD_CS 5
#define PIN_LCD_RST 17
#define PIN_LCD_DC 6
#define PIN_LCD_SCK 18
#define PIN_LCD_MOSI 19

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

// Colours for convenience
#define ST7735_BLACK 0x0000   // 0b 00000 000000 00000
#define ST7735_BLUE 0x001F    // 0b 00000 000000 11111
#define ST7735_GREEN 0x07E0   // 0b 00000 111111 00000
#define ST7735_RED 0xF800     // 0b 11111 000000 00000
#define ST7735_CYAN 0x07FF    // 0b 00000 111111 11111
#define ST7735_MAGENTA 0xF81F // 0b 11111 000000 11111
#define ST7735_YELLOW 0xFFE0  // 0b 11111 111111 00000
#define ST7735_WHITE 0xFFFF   // 0b 11111 111111 11111

void init_lcd_hardware();
void hal_setup(void);
void lcd_init();
void lcd_command(uint8_t cmd);
void lcd_data(uint8_t data);
void lcd_set_address(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
void lcd_set_pixel(uint8_t x, uint8_t y, uint16_t color);
void lcd_fill_screen(uint16_t color);
void lcd_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *buf);
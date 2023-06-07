#include "lvgl.h"
#include <pico/stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <pico/stdlib.h>
#include <pico/stdio.h>
#include "hardware/spi.h"

#include "bsp/board.h"
#pragma once

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
void lcd_init();
void lcd_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p);
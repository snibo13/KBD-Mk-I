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

#define ST7735_MADCTL_MY 0x80
#define ST7735_MADCTL_MX 0x40
#define ST7735_MADCTL_MV 0x20
#define ST7735_MADCTL_ML 0x10
#define ST7735_MADCTL_RGB 0x00
#define ST7735_MADCTL_BGR 0x08
#define ST7735_MADCTL_MH 0x04

#define ST7735_IS_160X128 1
#define ST7735_WIDTH 128
#define ST7735_HEIGHT 160
#define ST7735_XSTART 0
#define ST7735_YSTART 0
#define ST7735_ROTATION (ST7735_MADCTL_MX | ST7735_MADCTL_MY)

#define ST7735_NOP 0x00
#define ST7735_SWRESET 0x01
#define ST7735_RDDID 0x04
#define ST7735_RDDST 0x09

#define ST7735_SLPIN 0x10
#define ST7735_SLPOUT 0x11
#define ST7735_PTLON 0x12
#define ST7735_NORON 0x13

#define ST7735_INVOFF 0x20
#define ST7735_INVON 0x21
#define ST7735_DISPOFF 0x28
#define ST7735_DISPON 0x29
#define ST7735_CASET 0x2A
#define ST7735_RASET 0x2B
#define ST7735_RAMWR 0x2C
#define ST7735_RAMRD 0x2E

#define ST7735_PTLAR 0x30
#define ST7735_COLMOD 0x3A
#define ST7735_MADCTL 0x36

#define ST7735_FRMCTR1 0xB1
#define ST7735_FRMCTR2 0xB2
#define ST7735_FRMCTR3 0xB3
#define ST7735_INVCTR 0xB4
#define ST7735_DISSET5 0xB6

#define ST7735_PWCTR1 0xC0
#define ST7735_PWCTR2 0xC1
#define ST7735_PWCTR3 0xC2
#define ST7735_PWCTR4 0xC3
#define ST7735_PWCTR5 0xC4
#define ST7735_VMCTR1 0xC5

#define ST7735_RDID1 0xDA
#define ST7735_RDID2 0xDB
#define ST7735_RDID3 0xDC
#define ST7735_RDID4 0xDD

#define ST7735_PWCTR6 0xFC

#define ST7735_GMCTRP1 0xE0
#define ST7735_GMCTRN1 0xE1

// Color definitions
#define ST7735_BLACK 0x0000
#define ST7735_BLUE 0x001F
#define ST7735_RED 0xF800
#define ST7735_GREEN 0x07E0
#define ST7735_CYAN 0x07FF
#define ST7735_MAGENTA 0xF81F
#define ST7735_YELLOW 0xFFE0
#define ST7735_WHITE 0xFFFF
#define ST7735_ORANGE 0xFD60
#define ST7735_LIGHTGREEN 0x07EF
#define ST7735_COLOR565(r, g, b) (((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3))

#define BGCOLOR ST7735_BLACK
#define AXISCOLOR ST7735_ORANGE
#define LIGHTCOLOR ST7735_LIGHTGREEN

void init_lcd_hardware();
void hal_setup(void);
void lcd_init();
void lcd_command(uint8_t cmd);
void lcd_data(uint8_t data);
void lcd_set_address(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
void lcd_set_pixel(uint8_t x, uint8_t y, uint16_t color);
void lcd_fill_screen(uint16_t color);
void lcd_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *buf);
uint16_t color565(uint8_t r, uint8_t g, uint8_t b);

void ST7735_Init_Command1(void);
void ST7735_Init_Command2(void);
void ST7735_Init_Command3(void);
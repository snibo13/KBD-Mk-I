#pragma once

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

void lcd_init();

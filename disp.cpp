#include "disp.h"

void lcd_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t size = lv_area_get_width(area) * lv_area_get_height(area);

    lcd_set_address(area->x1, area->y1, area->x2, area->y2);

    gpio_put(PIN_LCD_DC, 1); // Set DC high for data mode
    spi_write_blocking(spi0, (const uint8_t *)color_p, size * sizeof(lv_color_t));

    lv_disp_flush_ready(disp_drv);
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
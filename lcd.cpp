#include "lcd.h"

#include "hardware/spi.h"
#include <pico/stdlib.h>
#include "lvgl.h"
#include "lv_conf.h"

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

void init_lcd_hardware(void)
{
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
    lcd_command(ST7735_COLOR_MODE);
    lcd_data(0x05);
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

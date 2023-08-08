#include "lcd.h"

#include "hardware/spi.h"
#include <pico/stdlib.h>
#include "lvgl.h"
#include "lv_conf.h"

uint16_t color565(uint8_t r, uint8_t g, uint8_t b)
{
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
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

void ST7735_Init_Command1(void)
{
    lcd_command(ST7735_SWRESET); //  1: Software reset
    sleep_ms(150);
    lcd_command(ST7735_SLPOUT); //  2: Out of sleep mode
    sleep_ms(500);
    lcd_command(ST7735_FRMCTR1); //  3: Frame rate ctrl - normal mode
    lcd_data(0x01);              //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
    lcd_data(0x2C);
    lcd_data(0x2D);
    lcd_command(ST7735_FRMCTR2); //  4: Frame rate control - idle mode
    lcd_data(0x01);              //  Rate = fosc/(1x2+40) * (LINE+2C+2D)
    lcd_data(0x2C);
    lcd_data(0x2D);
    lcd_command(ST7735_FRMCTR3); //  5: Frame rate ctrl - partial mode
    lcd_data(0x01);              //     Dot inversion mode
    lcd_data(0x2C);
    lcd_data(0x2D);
    lcd_data(0x01); //     Line inversion mode
    lcd_data(0x2C);
    lcd_data(0x2D);
    lcd_command(ST7735_INVCTR); //  6: Display inversion ctrl
    lcd_data(0x07);             //     No inversion
    lcd_command(ST7735_PWCTR1); //  7: Power control
    lcd_data(0xA2);
    lcd_data(0x02);             //     -4.6V
    lcd_data(0x84);             //     AUTO mode
    lcd_command(ST7735_PWCTR2); //  8: Power control
    lcd_data(0xC5);             //     VGH25 = 2.4C VGSEL = -10 VGH = 3 * AVDD
    lcd_command(ST7735_PWCTR3); //  9: Power control
    lcd_data(0x0A);             //     Opamp current small
    lcd_data(0x00);             //     Boost frequency
    lcd_command(ST7735_PWCTR4); // 10: Power control
    lcd_data(0x8A);             //     BCLK/2, Opamp current small & Medium low
    lcd_data(0x2A);
    lcd_command(ST7735_PWCTR5); // 11: Power control
    lcd_data(0x8A);
    lcd_data(0xEE);
    lcd_command(ST7735_VMCTR1); // 12: Power control
    lcd_data(0x0E);
    lcd_command(ST7735_INVOFF); // 13: Don't invert display
    lcd_command(ST7735_MADCTL); // 14: Memory access control (directions)
    lcd_data(ST7735_ROTATION);  //     row addr/col addr, bottom to top refresh
    lcd_command(ST7735_COLMOD); // 15: set color mode
    lcd_data(0x05);             //     16-bit color
}

void ST7735_Init_Command2(void)
{
    lcd_command(ST7735_CASET); //  1: Column addr set
    lcd_data(0x00);            //     XSTART = 0
    lcd_data(0x00);
    lcd_data(0x00); //     XEND = 127
    lcd_data(0x7F);
    lcd_command(ST7735_RASET); //  2: Row addr set
    lcd_data(0x00);            //     XSTART = 0
    lcd_data(0x00);
    lcd_data(0x00); //     XEND = 127
    lcd_data(0x7F);
}

void ST7735_Init_Command3(void)
{
    lcd_command(ST7735_GMCTRP1); //  1: Magical unicorn dust
    lcd_data(0x02);
    lcd_data(0x1C);
    lcd_data(0x07);
    lcd_data(0x12);
    lcd_data(0x37);
    lcd_data(0x32);
    lcd_data(0x29);
    lcd_data(0x2D);
    lcd_data(0x29);
    lcd_data(0x25);
    lcd_data(0x2B);
    lcd_data(0x39);
    lcd_data(0x00);
    lcd_data(0x01);
    lcd_data(0x03);
    lcd_data(0x10);
    lcd_command(ST7735_GMCTRN1); //  2: Sparkles and rainbows
    lcd_data(0x03);
    lcd_data(0x1D);
    lcd_data(0x07);
    lcd_data(0x06);
    lcd_data(0x2E);
    lcd_data(0x2C);
    lcd_data(0x29);
    lcd_data(0x2D);
    lcd_data(0x2E);
    lcd_data(0x2E);
    lcd_data(0x37);
    lcd_data(0x3F);
    lcd_data(0x00);
    lcd_data(0x00);
    lcd_data(0x02);
    lcd_data(0x10);
    lcd_command(ST7735_NORON);
    sleep_ms(10);
    lcd_command(ST7735_DISPON);
    sleep_ms(100);
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
    init_lcd_hardware();
    ST7735_Init_Command1();
    ST7735_Init_Command2();
    ST7735_Init_Command3();
    // // Send initialization commands to the LCD
    // lcd_command(CMD_SWRESET); // Software reset
    // sleep_ms(150);
    // lcd_command(CMD_SLPOUT); // Sleep out
    // sleep_ms(150);
    // lcd_command(CMD_DISPON); // Display on
    // sleep_ms(150);
    // lcd_command(ST7735_COLOR_MODE);
    // lcd_data(0x05);
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

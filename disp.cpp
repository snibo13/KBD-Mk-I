#include "disp.h"
#include "st7735.h"

#define write_data lcd_write_data
#define write_command lcd_command

void lcd_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
// Color array to framebuffer transfer
{
    lv_coord_t hres = disp_drv->hor_res;
    lv_coord_t vres = disp_drv->ver_res;

    if (area->x2 < 0 || area->y2 < 0 || area->x1 > hres - 1 || area->y1 > vres - 1)
    {
        lv_disp_flush_ready(disp_drv);
        return;
    }

    int32_t x, y;
    for (y = area->y1; y <= area->y2 && y < disp_drv->ver_res; y++)
    {
        for (x = area->x1; x <= area->x2; x++)
        {
            tft_fb[y * disp_drv->hor_res + x] = lv_color_to16(*color_p);
            color_p++;
        }
    }
    // lcd_update((int32_t)area->x1, (int32_t)area->y1, (int32_t)area->x2, (int32_t)area->y2);
    lcd_update();
    lv_disp_flush_ready(disp_drv);
}

void lcd_update()
// Framebuffer to LCD
{
    uint16_t color;
    lcd_set_address(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
    for (int i = 0; i < (LCD_WIDTH * LCD_HEIGHT); i++)
    {
        color = tft_fb[i];
        lcd_data(color >> 8);
        lcd_data(color);
    }
}

// void lcd_update(int32_t x1, int32_t y1, int32_t x2, int32_t y2)
// {
//     lcd_set_address(x1, y1, x2, y2);
//     uint8_t x, y;
//     for (y = y1 / 8; y <= y2 / 8; y++)
//     {
//         for (x = x1; x <= x2; x++)
//         {
//             lcd_set_pixel(x, y, tft_fb[y * LCD_WIDTH + x]);
//         }
//     }
// }

// Set data mode
// Write byte

void spi_write_color(uint16_t color)
{
    // Convert the color value to 8-bit components (assuming 5-6-5 RGB format)
    uint8_t r = (color >> 11) & 0x1F;
    uint8_t g = (color >> 5) & 0x3F;
    uint8_t b = color & 0x1F;

    // Write the color components to the SPI interface
    spi_write_byte(r);
    spi_write_byte(g);
    spi_write_byte(b);
}

void spi_write_byte(uint8_t byte)
{
    spi_write_blocking(spi0, &byte, sizeof(uint8_t));
}

void lcd_init()
{
    lcd_command(ST7735_SWRESET); // Reset
    board_delay(60);
    lcd_command(ST7735_SLPOUT);
    board_delay(250);
    board_delay(250);
    lcd_command(ST7735_COLMOD);
    lcd_data(0x05);
    board_delay(10);
    lcd_command(ST7735_FRMCTR1);
    lcd_write_data(0x00);
    lcd_write_data(0x06);
    write_data(0x03);
    board_delay(10);
    write_command(ST7735_MADCTL);
    write_data(0x08);
    write_command(ST7735_DISSET5);
    write_data(0x15);
    write_data(0x02);
    write_command(ST7735_INVCTR);
    write_data(0x00);
    write_command(ST7735_PWCTR1);
    write_data(0x02);
    write_data(0x70);
    board_delay(10);
    write_command(ST7735_PWCTR2);
    write_data(0x05);
    write_command(ST7735_PWCTR3);
    write_data(0x01);
    write_data(0x02);
    write_command(ST7735_VMCTR1);
    write_data(0x3C);
    write_data(0x38);
    board_delay(10);
    write_command(ST7735_PWCTR6);
    write_data(0x11);
    write_data(0x15);
    write_command(ST7735_GMCTRP1);
    write_data(0x09);
    write_data(0x16);
    write_data(0x09);
    write_data(0x20);
    write_data(0x21);
    write_data(0x1B);
    write_data(0x13);
    write_data(0x19);
    write_data(0x17);
    write_data(0x15);
    write_data(0x1E);
    write_data(0x2B);
    write_data(0x04);
    write_data(0x05);
    write_data(0x02);
    write_data(0x0E);
    write_command(ST7735_GMCTRN1);
    write_data(0x0B);
    write_data(0x14);
    write_data(0x08);
    write_data(0x1E);
    write_data(0x22);
    write_data(0x1D);
    write_data(0x18);
    write_data(0x1E);
    write_data(0x1B);
    write_data(0x1A);
    write_data(0x24);
    write_data(0x2B);
    write_data(0x06);
    write_data(0x06);
    write_data(0x02);
    write_data(0x0F);
    board_delay(10);
    write_command(ST7735_CASET);
    write_data(0x00);
    write_data(0x02);
    write_data(0x08);
    write_data(0x81);
    write_command(ST7735_RASET);
    write_data(0x00);
    write_data(0x01);
    write_data(0x08);
    write_data(0xA0);
    write_command(ST7735_NORON);
    board_delay(10);
    write_command(ST7735_DISPON);
    board_delay(250);
    board_delay(250);
}

// void lcd_init()
// {

//     // Initialize SPI interface
//     spi_init(spi0, 500000);
//     gpio_set_function(PIN_LCD_SCK, GPIO_FUNC_SPI);
//     gpio_set_function(PIN_LCD_MOSI, GPIO_FUNC_SPI);

//     // Configure LCD pin directions
//     gpio_init(PIN_LCD_CS);
//     gpio_set_dir(PIN_LCD_CS, GPIO_OUT);
//     gpio_init(PIN_LCD_RST);
//     gpio_set_dir(PIN_LCD_RST, GPIO_OUT);
//     gpio_init(PIN_LCD_DC);
//     gpio_set_dir(PIN_LCD_DC, GPIO_OUT);

//     // Reset the LCD
//     gpio_put(PIN_LCD_RST, 0);
//     sleep_ms(100);
//     gpio_put(PIN_LCD_RST, 1);
//     sleep_ms(100);

//     // Initialisation sequence
//     lcd_write_to_address(LCD_REG_17, 0x00);

//     lcd_write_to_address(LCD_REG_177);
//     lcd_write_data(0x01);
//     lcd_write_data(0x2c);
//     lcd_write_data(0x2d);

//     lcd_write_to_address(LCD_REG_178, 0x01);
//     lcd_write_to_address(LCD_REG_178, 0x2c);
//     lcd_write_to_address(LCD_REG_178, 0x2d);

//     lcd_write_to_address(LCD_REG_179, 0x01);
//     lcd_write_to_address(LCD_REG_179, 0x2c);
//     lcd_write_to_address(LCD_REG_179, 0x2d);
//     lcd_write_to_address(LCD_REG_179, 0x01);
//     lcd_write_to_address(LCD_REG_179, 0x2c);
//     lcd_write_to_address(LCD_REG_179, 0x2d);

//     lcd_write_to_address(LCD_REG_180, 0x01);

//     lcd_write_to_address(LCD_REG_192, 0xa2);
//     lcd_write_to_address(LCD_REG_192, 0x02);
//     lcd_write_to_address(LCD_REG_192, 0x84);

//     lcd_write_to_address(LCD_REG_193, 0xc5);

//     lcd_write_to_address(LCD_REG_194, 0x0a);
//     lcd_write_to_address(LCD_REG_194, 0x00);

//     lcd_write_to_address(LCD_REG_195, 0x8a);
//     lcd_write_to_address(LCD_REG_195, 0x2a);

//     lcd_write_to_address(LCD_REG_196, 0x8a);
//     lcd_write_to_address(LCD_REG_196, 0xee);

//     lcd_write_to_address(LCD_REG_197, 0x0e);

//     lcd_write_to_address(LCD_REG_32);

//     lcd_write_to_address(LCD_REG_58, 0x05);

//     lcd_write_to_address(LCD_REG_42);

//     lcd_write_data(0x00);
//     lcd_write_data(0x00);
//     lcd_write_data(0x00);
//     lcd_write_data(0x7F);

//     lcd_write_to_address(LCD_REG_43);

//     lcd_write_data(0x00);
//     lcd_write_data(0x00);
//     lcd_write_data(0x00);
//     lcd_write_data(0x9F);

//     lcd_write_to_address(LCD_REG_224, 0x02);
//     lcd_write_to_address(LCD_REG_224, 0x1c);
//     lcd_write_to_address(LCD_REG_224, 0x07);
//     lcd_write_to_address(LCD_REG_224, 0x12);
//     lcd_write_to_address(LCD_REG_224, 0x37);
//     lcd_write_to_address(LCD_REG_224, 0x32);
//     lcd_write_to_address(LCD_REG_224, 0x29);
//     lcd_write_to_address(LCD_REG_224, 0x25);
//     lcd_write_to_address(LCD_REG_224, 0x2B);
//     lcd_write_to_address(LCD_REG_224, 0x39);
//     lcd_write_to_address(LCD_REG_224, 0x00);
//     lcd_write_to_address(LCD_REG_224, 0x01);
//     lcd_write_to_address(LCD_REG_224, 0x03);
//     lcd_write_to_address(LCD_REG_224, 0x10);

//     lcd_write_to_address(LCD_REG_225, 0x03);
//     lcd_write_to_address(LCD_REG_225, 0x1d);
//     lcd_write_to_address(LCD_REG_225, 0x07);
//     lcd_write_to_address(LCD_REG_225, 0x06);
//     lcd_write_to_address(LCD_REG_225, 0x2E);
//     lcd_write_to_address(LCD_REG_225, 0x2C);
//     lcd_write_to_address(LCD_REG_225, 0x29);
//     lcd_write_to_address(LCD_REG_225, 0x2D);
//     lcd_write_to_address(LCD_REG_225, 0x2E);
//     lcd_write_to_address(LCD_REG_225, 0x2E);
//     lcd_write_to_address(LCD_REG_225, 0x37);
//     lcd_write_to_address(LCD_REG_225, 0x3F);
//     lcd_write_to_address(LCD_REG_225, 0x00);
//     lcd_write_to_address(LCD_REG_225, 0x00);
//     lcd_write_to_address(LCD_REG_225, 0x02);
//     lcd_write_to_address(LCD_REG_225, 0x10);
//     /* Normal display on, no args, no delay */
//     lcd_write_to_address(LCD_REG_19, 0x00);
//     /* Main screen turn on, no delay */
//     lcd_write_to_address(LCD_REG_41, 0x00);
//     /* Memory access control: MY = 1, MX = 1, MV = 0, ML = 0 */
//     lcd_write_to_address(LCD_REG_54, 0xC0);
// }

void lcd_setup()
{
    static lv_disp_draw_buf_t disp_buf;
    static lv_color_t buf[LCD_WIDTH * 10];                       /*Declare a buffer for 10 lines*/
    lv_disp_draw_buf_init(&disp_buf, buf, NULL, LCD_WIDTH * 10); /*Initialize the display buffer*/

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);   /*Basic initialization*/
    disp_drv.flush_cb = lcd_flush; /*Used when `LV_VDB_SIZE != 0` in lv_conf.h (buffered drawing)*/
    disp_drv.draw_buf = &disp_buf;
    disp_drv.hor_res = LCD_WIDTH;
    disp_drv.ver_res = LCD_HEIGHT;
    // disp_drv.disp_fill = monitor_fill;      /*Used when `LV_VDB_SIZE == 0` in lv_conf.h (unbuffered drawing)*/
    // disp_drv.disp_map = monitor_map;        /*Used when `LV_VDB_SIZE == 0` in lv_conf.h (unbuffered drawing)*/
    lv_disp_drv_register(&disp_drv);
}

void lcd_command(uint8_t cmd)
{
    gpio_put(PIN_LCD_DC, 0); // Set DC low for command mode
    gpio_put(PIN_LCD_CS, 0);
    spi_write_blocking(spi0, &cmd, 1);
    gpio_put(PIN_LCD_CS, 1);
}

void lcd_data(uint8_t data)
{
    gpio_put(PIN_LCD_DC, 1); // Set DC high for data mode
    gpio_put(PIN_LCD_CS, 0);
    spi_write_blocking(spi0, &data, 1);
    gpio_put(PIN_LCD_CS, 1);
}

void lcd_write_to_address(uint8_t reg)
{
    spi_write_blocking(spi0, &reg, 1);
}

void lcd_write_to_address(uint8_t reg, uint8_t data)
{
    spi_write_blocking(spi0, &reg, 1);
    spi_write_blocking(spi0, &data, 1);
}

void lcd_write_data(uint8_t data)
{
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
    // lcd_set_address(x, y, x, y);
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

void lcd_fill_screen_two(uint16_t color)
{
    lcd_set_address(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
    for (int x = 0; x < (LCD_WIDTH); x++)
    {
        for (int y = 0; y < (LCD_HEIGHT); y++)
        {
            lcd_set_pixel(x, y, color);
        }
    }
}
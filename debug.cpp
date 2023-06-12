#include <stdlib.h>
#include <stdio.h>
#include "pico/stdio.h"
#include "pico/stdlib.h"
#include "bsp/board.h"
#include "hardware/spi.h"

#include "commands.h"

#define CS 5
#define CLK 18
#define MOSI 19
#define MISO 16
#define DC 6
#define RST 17

void lcd_command(uint8_t cmd);
void lcd_data(uint8_t data);

const uint8_t width = 128;
const uint8_t height = 160;

int main(void)
{

    board_init();
    board_led_off();
    // Initialise SPI
    spi_init(spi0, 1000000); // 1 MHz

    gpio_put(CS, 0);

    // Initialising SPI ports
    gpio_set_function(CS, GPIO_FUNC_SIO);
    gpio_set_dir(CS, GPIO_OUT);
    gpio_set_dir(DC, GPIO_OUT);
    gpio_set_dir(RST, GPIO_OUT);

    lcd_command(ST7735_SW_RESET);
    sleep_ms(150);
    lcd_command(ST7735_SLEEP_OUT);
    sleep_ms(50);

    lcd_command(ST7735_FRAME_RATE_CTRL1);
    lcd_data(0x01);
    lcd_data(0x2c);
    lcd_data(0x2d);

    lcd_command(ST7735_FRAME_RATE_CTRL2);
    lcd_data(0x01);
    lcd_data(0x2c);
    lcd_data(0x2d);

    lcd_command(ST7735_FRAME_RATE_CTRL3);
    lcd_data(0x01);
    lcd_data(0x2c);
    lcd_data(0x2d);
    lcd_data(0x01);
    lcd_data(0x2c);
    lcd_data(0x2d);

    lcd_command(ST7735_FRAME_INVERSION_CTRL);
    lcd_data(0x07);

    lcd_command(ST7735_PWR_CTRL1);
    lcd_data(0xa2);
    lcd_data(0x02);
    lcd_data(0x84);

    lcd_command(ST7735_PWR_CTRL2);
    lcd_data(0x0a);
    lcd_data(0x00);

    lcd_command(ST7735_PWR_CTRL4);
    lcd_data(0x8a);
    lcd_data(0x2a);

    lcd_command(ST7735_PWR_CTRL5);
    lcd_data(0x8a);
    lcd_data(0xee);

    lcd_data(ST7735_VMOF_CTRL);
    lcd_data(0x0e);

    lcd_command(ST7735_MADCTL);
    lcd_data(0xC0);

    lcd_command(ST7735_COLOR_MODE);
    lcd_data(0x05);

    lcd_command(ST7735_CASET);
    lcd_data(0x00);
    lcd_data(0x00);
    lcd_data(0x00);
    lcd_data(width);

    lcd_command(ST7735_RASET);
    lcd_data(0x00);
    lcd_data(0x00);
    lcd_data(0x00);
    lcd_data(height);

    lcd_command(ST7735_PV_GAMMA_CTRL);
    lcd_data(0x02);
    lcd_data(0x1c);
    lcd_data(0x07);
    lcd_data(0x12);
    lcd_data(0x37);
    lcd_data(0x32);
    lcd_data(0x29);
    lcd_data(0x2d);
    lcd_data(0x29);
    lcd_data(0x25);
    lcd_data(0x2B);
    lcd_data(0x39);
    lcd_data(0x00);
    lcd_data(0x01);
    lcd_data(0x03);
    lcd_data(0x10);

    lcd_command(ST7735_NV_GAMMA_CTRL);
    lcd_data(0x03);
    lcd_data(0x1d);
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

    lcd_command(ST7735_NORMAL_DISPLAY_OFF);
    sleep_ms(10);

    lcd_command(ST7735_DISPLAY_ON);
    sleep_ms(100);

    // set_window
    lcd_command(ST7735_CASET);
    lcd_data(0 >> 8);
    lcd_data(0);
    lcd_data((width - 1) >> 8);
    lcd_data(width - 1);
    lcd_command(ST7735_RASET);
    lcd_data(0 >> 8);
    lcd_data(0);
    lcd_data((height - 1) >> 8);
    lcd_data(height - 1);

    // Memory write mode

    gpio_put(DC, 0); // setCommandMode();
    uint8_t tmp = 0x2A;
    spi_write_blocking(spi0, &tmp, 1);
    // sendCommand(0x2A);  // Set column address
    gpio_put(DC, 1); // setDataMode();
    const uint8_t column_data[] = {0x00, 0x00, (width - 1) >> 8, (width - 1) & 0xFF};
    spi_write_blocking(spi0, column_data, sizeof(column_data));

    gpio_put(DC, 0); // setCommandMode();
    tmp = 0x2B;
    spi_write_blocking(spi0, &tmp, 1);
    // sendCommand(0x2B); // Set row address
    gpio_put(DC, 1);
    // setDataMode();
    uint8_t row_data[] = {0x00, 0x00, (height - 1) >> 8, (height - 1) & 0xFF};
    spi_write_blocking(spi0, row_data, sizeof(row_data));

    // setCommandMode();
    gpio_put(DC, 1);
    tmp = 0x2C;
    spi_write_blocking(spi0, &tmp, 1);
    // sendCommand(0x2C); // Memory write
    gpio_put(DC, 1);

    // setDataMode();

    uint16_t color = 0x0000;

    uint8_t high_byte = (color >> 8) & 0xFF;
    uint8_t low_byte = color & 0xFF;

    for (int i = 0; i < width * height; i++)
    {
        spi_write_blocking(spi0, &high_byte, 1);
        spi_write_blocking(spi0, &low_byte, 1);
    }

    // lcd_command(ST7735_WRITE_RAM);

    // for (int i = 0; i < (height - 1) * (width - 1); i++)
    // {
    //     lcd_data(high_byte);
    //     lcd_data(low_byte);
    // }

    // gpio_put(CS, 1);

    while (1)
    {
        board_led_on();
        sleep_ms(500);
        board_led_off();
        sleep_ms(500);
    }

    return -1;
}

void lcd_command(uint8_t cmd)
{
    // gpio_put(CS, 0);
    gpio_put(DC, 0);
    spi_write_blocking(spi0, &cmd, 1);
    // gpio_put(CS, 1);
}

void lcd_data(uint8_t data)
{
    // gpio_put(CS, 0);
    gpio_put(DC, 1);
    spi_write_blocking(spi0, &data, 1);
    // gpio_put(CS, 1);
}
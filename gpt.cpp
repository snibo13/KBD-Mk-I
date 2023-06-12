#include "pico/stdlib.h"
#include "hardware/spi.h"

// SPI configuration
spi_inst_t *SPI_PORT = spi0;
const uint SPI_SPEED = 1000000; // 1MHz

// Chip select (CS) pin
const uint CS_PIN = 5;

// Data/Command (DC) pin
const uint DC_PIN = 6;

// Display dimensions
const uint16_t SCREEN_WIDTH = 128;
const uint16_t SCREEN_HEIGHT = 160;

// Color definitions
const uint16_t COLOR_BLACK = 0x0000;
const uint16_t COLOR_WHITE = 0xFFFF;

void selectDevice()
{
    gpio_put(CS_PIN, 0);
}

void deselectDevice()
{
    gpio_put(CS_PIN, 1);
}

void setCommandMode()
{
    gpio_put(DC_PIN, 0);
}

void setDataMode()
{
    gpio_put(DC_PIN, 1);
}

void sendCommand(uint8_t command)
{
    setCommandMode();
    spi_write_blocking(SPI_PORT, &command, 1);
}

void sendData(uint8_t *data, size_t len)
{
    setDataMode();
    spi_write_blocking(SPI_PORT, data, len);
}

void fillScreen(uint16_t color)
{
    uint8_t high_byte = (color >> 8) & 0xFF;
    uint8_t low_byte = color & 0xFF;

    setCommandMode();
    sendCommand(0x2A); // Set column address
    setDataMode();
    uint8_t column_data[] = {0x00, 0x00, (SCREEN_WIDTH - 1) >> 8, (SCREEN_WIDTH - 1) & 0xFF};
    sendData(column_data, sizeof(column_data));

    setCommandMode();
    sendCommand(0x2B); // Set row address
    setDataMode();
    uint8_t row_data[] = {0x00, 0x00, (SCREEN_HEIGHT - 1) >> 8, (SCREEN_HEIGHT - 1) & 0xFF};
    sendData(row_data, sizeof(row_data));

    setCommandMode();
    sendCommand(0x2C); // Memory write

    setDataMode();
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++)
    {
        spi_write_blocking(SPI_PORT, &high_byte, 1);
        spi_write_blocking(SPI_PORT, &low_byte, 1);
    }
}

int main()
{
    // Initialize SPI
    spi_init(SPI_PORT, SPI_SPEED);
    gpio_set_function(CS_PIN, GPIO_FUNC_SIO);
    gpio_set_dir(CS_PIN, GPIO_OUT);
    gpio_set_function(DC_PIN, GPIO_FUNC_SIO);
    gpio_set_dir(DC_PIN, GPIO_OUT);

    // Select the device
    selectDevice();

    // Fill the screen with a specific color
    fillScreen(COLOR_BLACK);

    // Deselect the device
    deselectDevice();

    return 0;
}

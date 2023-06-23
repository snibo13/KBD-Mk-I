#include "stdio.h"
#include <pico/stdlib.h>

#define SHLD 22
#define CLK 14
#define DATA 12

void initialise_register(void);
void load_to_register(void);
uint8_t read_register(void);

int main(void)
{
    // Initialise stdio
    stdio_init_all();
    sleep_ms(5000);

    // Initial states
    initialise_register();
    for (int j = 0; j < 20; ++j)
    {
        // Read
        load_to_register();

        // Shift in
        read_register();
        printf("Done.\n");
        sleep_ms(1000);
    }
    printf("Done.\n");
}

void initialise_register(void)
{
    gpio_init(SHLD);
    gpio_init(CLK);
    gpio_init(DATA);

    gpio_set_dir(SHLD, GPIO_OUT);
    gpio_set_dir(CLK, GPIO_OUT);
    gpio_set_dir(DATA, GPIO_IN);

    gpio_put(CLK, 1);
    sleep_us(5);
    gpio_put(SHLD, 1);
    sleep_us(5);

    sleep_ms(1000);
}

void load_to_register(void)
{

    gpio_put(SHLD, 0);
    sleep_us(5);
    gpio_put(SHLD, 1);
    sleep_us(5);

    gpio_put(CLK, 1);
}

uint8_t read_register(void)
{
    uint8_t value = 0;
    uint8_t i;
    uint8_t bit;

    for (i = 0; i < 8; ++i)
    {
        gpio_put(CLK, 1);
        bit = gpio_get(DATA);
        printf("%d\t", bit);
        value |= bit << i;
        gpio_put(CLK, 0);
    }

    return value;
}
#include "shift_register.h"

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
    sleep_us(50);
    gpio_put(SHLD, 1);
    sleep_us(50);

    gpio_put(CLK, 1);
    sleep_us(50);
}

uint8_t read_register(void)
{
    uint8_t value = 0;
    uint8_t i, bit;
    for (i = 0; i < 8; ++i)
    {
        gpio_put(CLK, 1);
        bit = gpio_get(DATA);
        printf("%d\t", bit);
        sleep_us(50);
        value |= bit << i;
        gpio_put(CLK, 0);
        sleep_us(50);
    }

    return value;
}
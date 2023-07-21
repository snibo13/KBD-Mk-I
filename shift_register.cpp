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
    // sleep_us(3);
    gpio_put(SHLD, 1);
    // sleep_us(3);

    sleep_ms(1000);
}

void load_to_register(void)
{

    gpio_put(SHLD, 0);
    // sleep_us(3);
    gpio_put(SHLD, 1);
    // sleep_us(3);

    gpio_put(CLK, 1);
    // sleep_us(3);
}

void read_register(uint8_t *value)
{
    uint8_t i, bit, r;
    for (r = 0; r < NUM_REGISTERS; r++)
    {
        for (i = 0; i < 8; ++i)
        {
            bit = gpio_get(DATA);
            // printf("%d\t", bit);
            value[r] |= bit << i;
            gpio_put(CLK, 0);
            sleep_us(1);
            gpio_put(CLK, 1);
            sleep_us(1);
        }
    }
    // printf("\n");
}

void register_task()
{
    // Polls and stores current register state
    load_to_register();
    // read_register();
}
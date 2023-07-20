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

uint16_t read_register(void)
{
    uint16_t value = 0;
    uint8_t i, bit, r;
    for (r = 0; r < NUM_REGISTERS; r++)
    {
        for (i = 0; i < 8; ++i)
        {
            bit = gpio_get(DATA);
            printf("%d\t", bit);
            value |= bit << i;
            gpio_put(CLK, 0);
            sleep_us(1);
            gpio_put(CLK, 1);
            sleep_us(1);
        }
    }

    return value;
}

void register_task()
{
    // Polls and stores current register state
    load_to_register();
    uint8_t register_state = 0;
    for (uint8_t register_num = NUM_REGISTERS; register_num > 0; register_num--)
    {
        uint8_t current_register = read_register();
        keyboard_state[register_num] = current_register;
    }
}
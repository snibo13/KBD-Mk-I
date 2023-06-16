#include "shift_register.h"

void init_shift_register(void)
{
    gpio_set_dir(QH, GPIO_IN);
    gpio_set_dir(LD, GPIO_OUT);
    gpio_set_dir(CLK, GPIO_OUT);
}

shift_register_t *read_shift_register(void)
{
    uint8_t inputs = 0;
    bool bit = 0;
    shift_register_t *r;
    gpio_put(CLK, 1); // CLK high
    sleep_ms(1);
    gpio_put(LD, 1); // Enable shifting of data
    for (int reg = 0; reg < REGISTER_CNT; reg++)
    {
        bit = gpio_get(QH);
        r->a = bit;
        // Toggle CLOCK
        gpio_put(CLK, 0); // CLK high
        sleep_ms(1);
        gpio_put(CLK, 1); // CLK high
        sleep_ms(1);

        bit = gpio_get(QH);
        r->b = bit;

        // Toggle CLOCK
        gpio_put(CLK, 0); // CLK high
        sleep_ms(1);
        gpio_put(CLK, 1); // CLK high
        sleep_ms(1);

        bit = gpio_get(QH);
        r->c = bit;

        // Toggle CLOCK
        gpio_put(CLK, 0); // CLK high
        sleep_ms(1);
        gpio_put(CLK, 1); // CLK high
        sleep_ms(1);

        bit = gpio_get(QH);
        r->d = bit;

        // Toggle CLOCK
        gpio_put(CLK, 0); // CLK high
        sleep_ms(1);
        gpio_put(CLK, 1); // CLK high
        sleep_ms(1);

        bit = gpio_get(QH);
        r->e = bit;

        // Toggle CLOCK
        gpio_put(CLK, 0); // CLK high
        sleep_ms(1);
        gpio_put(CLK, 1); // CLK high
        sleep_ms(1);

        bit = gpio_get(QH);
        r->f = bit;

        // Toggle CLOCK
        gpio_put(CLK, 0); // CLK high
        sleep_ms(1);
        gpio_put(CLK, 1); // CLK high
        sleep_ms(1);

        bit = gpio_get(QH);
        r->g = bit;

        // Toggle CLOCK
        gpio_put(CLK, 0); // CLK high
        sleep_ms(1);
        gpio_put(CLK, 1); // CLK high
        sleep_ms(1);

        bit = gpio_get(QH);
        r->h = bit;
    }

    gpio_put(LD, 0); // Enable input of data
}

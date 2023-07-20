#include "stdio.h"
#include <pico/stdlib.h>

#include "shift_register.h"

static uint8_t val;
void print_byte(uint8_t val);
int main(void)
{
    // Initialise stdio
    stdio_init_all();
    sleep_ms(5000);

    // Initial states
    initialise_register();
    while (true)
    {
        // Read
        load_to_register();

        // Shift in
        read_register();
        // print_byte(val);

        printf("Done.\n");
        sleep_ms(1000);
    }
    printf("Done.\n");
}

void print_byte(uint8_t val)
{
    printf("Printing...\n");
    uint8_t i;
    for (uint8_t i = 0; i <= 7; i++)
    {
        printf("%d\t", val >> i & 1); // Magic bit shift, if you care look up the <<, >>, and & operators
    }
    printf("\n"); // Go to the next line, do not collect $200
}
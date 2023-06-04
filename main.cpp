// #include "usb.h"
#include <pico/stdlib.h>
#include <pico/stdio.h>

#include <tusb.h>
#include "usb_descriptors.h"

int main(void)
{
    tusb_init();
    stdio_init_all();

    while (1)
    {
        tud_task();
    }
    return 0;
}
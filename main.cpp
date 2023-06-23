// #include "usb.h"
#include <stdlib.h>
#include <stdio.h>
#include <pico/stdlib.h>
#include <pico/stdio.h>
#include "hardware/spi.h"
#include "hardware/uart.h"

#include "bsp/board.h"
#include "tusb.h"
#include "usb_descriptors.h"

#include "hid.h"
#include "universe.h"

#include "lv_conf.h"
#include "lvgl.h"

#include "lcd.h"
#include "shift_register.h"

// Function prototypes
static uint32_t blink_interval_ms = BLINK_UNMOUNTED;
static uint32_t sample_interval_ms = 1000;
static shift_register_t reg;

void register_task();

uint16_t color565(uint8_t r, uint8_t g, uint8_t b)
{
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

void led_task();
static int profile_number = 3;
int main(void)
{

    // uart_init(uart0, 115200);
    // gpio_set_function(0, GPIO_FUNC_UART);
    // gpio_set_function(1, GPIO_FUNC_UART);

    board_init();
    stdio_init_all();
    // tusb_init();

    init_lcd_hardware();
    // Initialize the LCD
    lcd_init();
    lv_init();
    hal_setup();
    init_shift_register();

    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_make(0, 255, 0), LV_PART_MAIN);

    // lv_obj_t *profile_label = lv_label_create(lv_scr_act());

    // char profile_char = (char)(48 + profile_number);
    // char profile_text[10] = "Profile 1";
    // profile_text[8] = profile_char;

    // lv_label_set_text(profile_label, profile_text);
    // lv_obj_set_style_text_color(lv_scr_act(), lv_color_hex(0xffffff), LV_PART_MAIN);
    // lv_obj_align(profile_label, LV_ALIGN_TOP_LEFT, 0, 0);

    lv_obj_t *reg_label = lv_label_create(lv_scr_act());
    lv_obj_align(reg_label, LV_ALIGN_CENTER, 0, 0);
    char bin_reg[8];
    printf("Running...");
    while (true)
    {
        printf("Loop\n");

        tud_task();
        // led_task();
        // profile_number = 1;
        // char profile_char = (char)(48 + profile_number);
        // profile_text[8] = profile_char;

        // lv_label_set_text(profile_label, profile_text);
        hid_task();
        register_task();
        // Building string
        bin_reg[0] = reg.a ? '1' : '0';
        bin_reg[1] = reg.b ? '1' : '0';
        bin_reg[2] = reg.c ? '1' : '0';
        bin_reg[3] = reg.d ? '1' : '0';
        bin_reg[4] = reg.e ? '1' : '0';
        bin_reg[5] = reg.f ? '1' : '0';
        bin_reg[6] = reg.g ? '1' : '0';
        bin_reg[7] = reg.h ? '1' : '0';
        lv_label_set_text(reg_label, bin_reg);
        lv_task_handler();
    }
    return 0;
}

// Device callbacks
void tud_mount_cb(void)
{
    blink_interval_ms = BLINK_MOUNTED;
}

void tud_unmount_cb(void)
{
    blink_interval_ms = BLINK_UNMOUNTED;
}

void tud_suspend_cb(bool remote_wakeup_en)
{
    (void)remote_wakeup_en;
    blink_interval_ms = BLINK_SUSPENDED;
}

void tud_resume_cb(void)
{
    blink_interval_ms = BLINK_MOUNTED;
}

// Debugging
void led_task(void)
{
    static uint32_t start_ms = 0;
    static bool led_state = false;

    if (!blink_interval_ms)
        return;

    // Blink every interval ms
    if (board_millis() - start_ms < blink_interval_ms)
        return; // not enough time
    start_ms += blink_interval_ms;

    board_led_write(led_state);
    led_state = 1 - led_state; // toggle
}

void register_task()
{
    static uint32_t sample_ms = 0;

    if (board_millis() - sample_ms < sample_interval_ms)
        return;
    sample_ms += sample_interval_ms;
    read_shift_register(&reg);
    // printf("Sample start\n");
    // printf("%d\n", reg.a);
    // printf("%d\n", reg.b);
    // printf("%d\n", reg.c);
    // printf("%d\n", reg.d);
    // printf("%d\n", reg.e);
    // printf("%d\n", reg.f);
    // printf("%d\n", reg.g);
    // printf("%d\n", reg.h);
    // printf("Sample End\n\n");

    // Sample register every sample_interval_ms milliseconds
}
// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const *buffer, uint16_t bufsize)
{
    (void)instance;

    if (report_type == HID_REPORT_TYPE_OUTPUT)
    {
        // Set keyboard LED e.g Capslock, Numlock etc...
        if (report_id == REPORT_ID_KEYBOARD)
        {
            // bufsize should be (at least) 1
            if (bufsize < 1)
                return;

            uint8_t const kbd_leds = buffer[0];

            if (kbd_leds & KEYBOARD_LED_CAPSLOCK)
            {
                // Capslock On: disable blink, turn led on
                blink_interval_ms = 0;
                board_led_write(true);
            }
            else
            {
                // Caplocks Off: back to normal blink
                board_led_write(false);
                blink_interval_ms = BLINK_MOUNTED;
            }
        }
    }
}

#include "hid.h"

// #include "usb.h"
#include <stdlib.h>
#include <stdio.h>
#include <pico/stdlib.h>
#include <pico/stdio.h>

#include "bsp/board.h"
#include "tusb.h"
#include "usb_descriptors.h"
#include "universe.h"

// HID

static void send_hid_report(uint8_t report_id, uint32_t btn)
{
    // skip if hid is not ready yet
    if (!tud_hid_ready())
        return;

    (void)report_id;
    // switch (report_id)
    // {
    // case REPORT_ID_KEYBOARD:
    // {
    // use to avoid send multiple consecutive zero report for keyboard
    static bool has_keyboard_key = false;

    if (btn)
    {
        uint8_t keycode[6] = {0};
        keycode[0] = HID_KEY_A;

        tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, keycode);
        has_keyboard_key = true;
    }
    else
    {
        // send empty key report if previously has key pressed
        if (has_keyboard_key)
            tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, NULL);
        has_keyboard_key = false;
    }
    // }
    // break;

    // case REPORT_ID_MOUSE:
    // {
    //     int8_t const delta = 5;

    //     // no button, right + down, no scroll, no pan
    //     tud_hid_mouse_report(REPORT_ID_MOUSE, 0x00, delta, delta, 0, 0);
    // }
    // break;

    // case REPORT_ID_CONSUMER_CONTROL:
    // {
    //     // use to avoid send multiple consecutive zero report
    //     static bool has_consumer_key = false;

    //     if (btn)
    //     {
    //         // volume down
    //         uint16_t volume_down = HID_USAGE_CONSUMER_VOLUME_DECREMENT;
    //         tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &volume_down, 2);
    //         has_consumer_key = true;
    //     }
    //     else
    //     {
    //         // send empty key report (release key) if previously has key pressed
    //         uint16_t empty_key = 0;
    //         if (has_consumer_key)
    //             tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &empty_key, 2);
    //         has_consumer_key = false;
    //     }
    // }
    // break;

    // case REPORT_ID_GAMEPAD:
    // {
    //     // use to avoid send multiple consecutive zero report for keyboard
    //     static bool has_gamepad_key = false;

    //     hid_gamepad_report_t report =
    //         {
    //             .x = 0, .y = 0, .z = 0, .rz = 0, .rx = 0, .ry = 0, .hat = 0, .buttons = 0};

    //     if (btn)
    //     {
    //         report.hat = GAMEPAD_HAT_UP;
    //         report.buttons = GAMEPAD_BUTTON_A;
    //         tud_hid_report(REPORT_ID_GAMEPAD, &report, sizeof(report));

    //         has_gamepad_key = true;
    //     }
    //     else
    //     {
    //         report.hat = GAMEPAD_HAT_CENTERED;
    //         report.buttons = 0;
    //         if (has_gamepad_key)
    //             tud_hid_report(REPORT_ID_GAMEPAD, &report, sizeof(report));
    //         has_gamepad_key = false;
    //     }
    // }
    // break;

    // default:
    //     break;
    // }
}

// Send 1 report for each HID profile
void hid_task(void)
{
    const uint32_t interval_ms = 10;
    static uint32_t start_ms = 0;

    if (board_millis() - start_ms < interval_ms)
        return;

    start_ms += interval_ms;

    uint32_t const btn = board_button_read();

    // Remote wakeup
    if (tud_suspended() && btn)
        tud_remote_wakeup;
    else
        send_hid_report(REPORT_ID_KEYBOARD, btn);
}

// Called when a sucessful HID report is sent to the host
// Message delivered confirmation
void tud_hid_report_complete_cb(uint8_t instance, uint8_t const *report, uint16_t len)
{
    (void)instance;
    (void)len;

    uint8_t next_report_id = report[0] + 1;
    if (next_report_id < REPORT_ID_COUNT)
    {
        send_hid_report(next_report_id, board_button_read());
    }
}
// Invoked when report is requested
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t *buffer, uint16_t reqlen)
{
    (void)instance;
    (void)report_id;
    (void)report_type;
    (void)buffer;
    (void)reqlen;

    return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
// void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const *buffer, uint16_t bufsize)
// {
//     (void)instance;

//     if (report_type == HID_REPORT_TYPE_OUTPUT)
//     {
//         // Set keyboard LED e.g Capslock, Numlock etc...
//         if (report_id == REPORT_ID_KEYBOARD)
//         {
//             // bufsize should be (at least) 1
//             if (bufsize < 1)
//                 return;

//             uint8_t const kbd_leds = buffer[0];

//             if (kbd_leds & KEYBOARD_LED_CAPSLOCK)
//             {
//                 // Capslock On: disable blink, turn led on
//                 blink_interval_ms = 0;
//                 board_led_write(true);
//             }
//             else
//             {
//                 // Caplocks Off: back to normal blink
//                 board_led_write(false);
//                 blink_interval_ms = BLINK_MOUNTED;
//             }
//         }
//     }
// }
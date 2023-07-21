#include "keyboard.h"
uint32_t checkBit(uint16_t value, uint8_t pos)
{
    uint32_t temp = value >> pos & 1;
    return temp;
}

void keyboard_task()
{
    // if (board_millis() - start_ms < task_interval_ms)
    //     return;
    // start_ms += task_interval_ms;
    hid_keyboard_report_t keyboard_report = {0};
    load_to_register();
    uint8_t *keyboard_state = new uint8_t[NUM_REGISTERS];
    memset(keyboard_state, 0, NUM_REGISTERS * sizeof(uint8_t));
    read_register(keyboard_state);
    // printf("%d\n\n", keyboard_state);
    uint32_t pressed;
    bool key_pressed = false;
    for (int r = 0; r < NUM_REGISTERS; r++)
    {
        // printf("Register %d\n", r);
        for (int i = 0; i < 8; i++)
        {
            pressed = checkBit(keyboard_state[r], i);
            if (pressed)
            {
                keyboard_report.keycode[0] |= keymap[r][i];
                key_pressed = true;
            }
            // printf("%d\t", temp);
        }
        // printf("\n");
    }

    // printf("\n");

    delete[] keyboard_state;
    send_hid_report(REPORT_ID_KEYBOARD, key_pressed);

    //     has_new_key = true;
    // } else {
    //     if (has_new_key)
    //         tud_h
    // }
}

static void send_hid_report(uint8_t report_id, uint32_t btn)
{
    // skip if hid is not ready yet
    if (!tud_hid_ready())
        return;

    switch (report_id)
    {
    case REPORT_ID_KEYBOARD:
    {
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
    }
    break;

    case REPORT_ID_MOUSE:
    {
        int8_t const delta = 5;

        // no button, right + down, no scroll, no pan
        tud_hid_mouse_report(REPORT_ID_MOUSE, 0x00, delta, delta, 0, 0);
    }
    break;

    case REPORT_ID_CONSUMER_CONTROL:
    {
        // use to avoid send multiple consecutive zero report
        static bool has_consumer_key = false;

        if (btn)
        {
            // volume down
            uint16_t volume_down = HID_USAGE_CONSUMER_VOLUME_DECREMENT;
            tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &volume_down, 2);
            has_consumer_key = true;
        }
        else
        {
            // send empty key report (release key) if previously has key pressed
            uint16_t empty_key = 0;
            if (has_consumer_key)
                tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &empty_key, 2);
            has_consumer_key = false;
        }
    }
    break;

    case REPORT_ID_GAMEPAD:
    {
        // use to avoid send multiple consecutive zero report for keyboard
        static bool has_gamepad_key = false;

        hid_gamepad_report_t report =
            {
                .x = 0, .y = 0, .z = 0, .rz = 0, .rx = 0, .ry = 0, .hat = 0, .buttons = 0};

        if (btn)
        {
            report.hat = GAMEPAD_HAT_UP;
            report.buttons = GAMEPAD_BUTTON_A;
            tud_hid_report(REPORT_ID_GAMEPAD, &report, sizeof(report));

            has_gamepad_key = true;
        }
        else
        {
            report.hat = GAMEPAD_HAT_CENTERED;
            report.buttons = 0;
            if (has_gamepad_key)
                tud_hid_report(REPORT_ID_GAMEPAD, &report, sizeof(report));
            has_gamepad_key = false;
        }
    }
    break;

    default:
        break;
    }
}
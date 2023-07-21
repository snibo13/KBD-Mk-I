#include "keyboard.h"
uint32_t checkBit(uint16_t value, uint8_t pos)
{
    uint32_t temp = value >> pos & 1;
    return temp;
}

void keyboard_task()
{
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
    uint8_t keycode[8] = {0};
    keycode[0] = HID_KEY_A;
    keycode[1] = HID_KEY_B;
    // skip if hid is not ready yet
    if (!tud_hid_ready())
        return;
    // use to avoid send multiple consecutive zero report for keyboard
    static bool has_keyboard_key = false;

    if (key_pressed)
    {
        // uint8_t keycode[6] = {0};
        // keycode[0] = HID_KEY_A;

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

static void send_hid_report(uint8_t report_id, uint32_t btn)
{
    // skip if hid is not ready yet
    if (!tud_hid_ready())
        return;
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

// Invoked when sent REPORT successfully to host
// Application can use this to send the next report
// Note: For composite reports, report[0] is report ID
void tud_hid_report_complete_cb(uint8_t instance, uint8_t const *report, uint16_t len)
{
    (void)instance;
    (void)len;
}

// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t *buffer, uint16_t reqlen)
{
    // TODO not Implemented
    (void)instance;
    (void)report_id;
    (void)report_type;
    (void)buffer;
    (void)reqlen;

    return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const *buffer, uint16_t bufsize)
{
    (void)instance;
}

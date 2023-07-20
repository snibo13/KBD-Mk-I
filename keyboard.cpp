#include "keyboard.h"

void keyboard_task()
{
    // Converts keyboard state into keyboard_report
    uint8_t modifiers = 0;

    // Iterate though keys and update key_state;
    uint8_t key_state = 0;
    for (uint8_t nr = NUM_REGISTERS; nr > 0; nr--)
    {
        for (uint8_t mask = 0; mask < 8; mask++)
        {
            if (keyboard_state[nr] & mask)
            {
                key_state |= keymap[nr][mask];
            }
        }
    }
    tud_hid_keyboard_report(REPORT_ID_KEYBOARD, modifiers, &key_state);
}
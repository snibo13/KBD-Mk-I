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
    load_to_register();
    uint8_t *keyboard_state = new uint8_t[NUM_REGISTERS];
    memset(keyboard_state, 0, NUM_REGISTERS * sizeof(uint8_t));
    read_register(keyboard_state);
    printf("%d\n\n", keyboard_state);
    for (int r = 0; r < NUM_REGISTERS; r++)
    {
        printf("Register %d\n", r);
        for (int i = 0; i < 8; i++)
        {
            uint32_t temp = checkBit(keyboard_state[r], i);
            printf("%d\t", temp);
        }
        printf("\n");
    }

    printf("\n");
    delete[] keyboard_state;
    // if (!k1)
    // {
    //     board_led_off();
    // }

    // if (tud_suspended() && k1)
    // {
    //     tud_remote_wakeup();
    // }
    // else
    // {
    //     send_hid_report(REPORT_ID_KEYBOARD, k1);
    //     board_led_on();
    // }

    // hid_keyboard_report_t report = {0};
    // Converts keyboard state into keyboard_report
    // uint8_t modifiers = 0;

    // Iterate though keys and update key_state;
    // for (uint8_t mask = 0; mask < 15; mask++)
    // {
    //     if (CHECK_BIT(keyboard_state, mask))
    //     {

    //         report.keycode[0] |= keymap[mask];
    //     }
    // }
    // tud_hid_keyboard_report(0, modifiers, report.keycode);
}
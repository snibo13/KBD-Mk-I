#include <inttypes.h>
#include "shift_register.h"
#include <inttypes.h>
// #define CFG_TUD_HID
#include "bsp/board.h"
#include "usb_descriptors.h"
#include "tusb.h"
#include "hid.h"
#include "stdlib.h"
#include <bitset>

static hid_keyboard_report_t keyboard_report;

void keyboard_task();
#define NUM_KEYS 16

static const uint32_t task_interval_ms = 1000;
static uint32_t start_ms = 0;
static uint8_t keymap[16] = {
    // Register 1
    HID_KEY_1, HID_KEY_2, HID_KEY_3, HID_KEY_4, HID_KEY_5, HID_KEY_6, HID_KEY_7, HID_KEY_8,
    // {HID_KEY_GRAVE, HID_KEY_1, HID_KEY_TAB, HID_KEY_Q, HID_KEY_CAPS_LOCK, HID_KEY_A, HID_KEY_SHIFT_LEFT, HID_KEY_Z},
    // Register 2
    HID_KEY_Q, HID_KEY_W, HID_KEY_E, HID_KEY_R, HID_KEY_SHIFT_LEFT, HID_KEY_ALT_LEFT, HID_KEY_CONTROL_LEFT, HID_KEY_APPLICATION};


uint32_t checkBit(uint16_t value, uint8_t pos);
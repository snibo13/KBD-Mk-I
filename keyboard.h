#include <inttypes.h>
#include "universe.h"
#include "shift_register.h"
#include "tusb.h"
#include "hid_device.h"
#include "usb_descriptors.h"

static hid_keyboard_report_t keyboard_report;

void keyboard_task();
#define NUM_KEYS 16

static uint8_t keymap[NUM_REGISTERS][8] = {
    // Register 1
    {HID_KEY_GRAVE, HID_KEY_1, HID_KEY_TAB, HID_KEY_Q, HID_KEY_CAPS_LOCK, HID_KEY_A, HID_KEY_SHIFT_LEFT, HID_KEY_Z},
    // Register 2
    {}};
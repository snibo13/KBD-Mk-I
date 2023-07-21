#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <pico/stdlib.h>
#include <pico/stdio.h>

#include "bsp/board.h"
#include "tusb.h"
#include "usb_descriptors.h"

#include "universe.h"

void hid_task(void);
void tud_hid_report_complete_cb(uint8_t instance, uint8_t const *report, uint16_t len);
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t *buffer, uint16_t reqlen);
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const *buffer, uint16_t bufsize);
void led_blinking_task(void);
#pragma once

#include "stdio.h"
#include <pico/stdlib.h>

#define SHLD 22
#define CLK 14
#define DATA 12

#define NUM_REGISTERS 2

static uint8_t keyboard_state[NUM_REGISTERS];

void initialise_register(void);
void load_to_register(void);
uint16_t read_register(void);
void register_task(void);
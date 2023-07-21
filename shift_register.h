#pragma once

#include "stdio.h"
#include <pico/stdlib.h>

#define SHLD 22
#define CLK 14
#define DATA 12

#define NUM_REGISTERS 2

// static uint16_t keyboard_state;

void initialise_register(void);
void load_to_register(void);
void read_register(uint8_t *);
void register_task(void);
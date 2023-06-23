#pragma once

#include "stdio.h"
#include <pico/stdlib.h>

#define SHLD 22
#define CLK 14
#define DATA 12

void initialise_register(void);
void load_to_register(void);
uint8_t read_register(void);
#pragma once

#include <pico/stdlib.h>

#define QH 15
#define LD 22
#define CLK 26

#define REGISTER_CNT 1

typedef struct
{ // Register data bit field
    bool a;
    bool b;
    bool c;
    bool d;
    bool e;
    bool f;
    bool g;
    bool h;
} shift_register_t;

void init_shift_register(void);
shift_register_t *read_shift_register(void);

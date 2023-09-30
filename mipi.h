#pragma once

#include "stdio.h"
#include <pico/stdlib.h>

#define CLKP 1
#define CLKN 2
#define D1P 4
#define D1N 3
#define D0P 4
#define D0N 5
#define RESET 6

struct DifferentialPair
{
    int positive;
    int negative;
};

struct DifferentialPair clk = {CLKP, CLKN};

void initialise_mipi();
void write_differential_pair(struct differential_pair *pins);
void create_mipi_packet(uint8_t *mipi_packet, uint8_t data_type, uint8_t vc, uint8_t data_id, uint32_t paylod);
void write_mipi(uint8_t *packet);
uint differential_pair_to_mask(struct differential_pair *p);

#pragma once

#include <stdint.h>

// Number of chips on the board
#define CSPIKE_CHIPS 25
// Number of channels per chip
#define CSPIKE_CH_PER_CHIP 12
// Length of the "lanes"
#define CSPIKE_LENGTH 60
// Number of LEDs in total
#define CSPIKE_LEDS (CSPIKE_CHIPS * CSPIKE_CH_PER_CHIP)

/**
 * The LED board is broken up into three "tracks"-- the RGB LEDs in the middle, the "hot" white lane,
 * and the "cool" light lane. */

typedef struct {
    int clk_pin;
    int data_pin;
    uint16_t leds[300];
} CSpike;


void cspike_init(const CSpike* b); // Should be called as soon as possible after power on!
void cspike_update(const CSpike* b); // Update with current data
void cspike_clear(CSpike* b); // Erase all
void cspike_set_hot(CSpike* b, uint8_t which, uint16_t value); // Set the value on the hot rail
void cspike_set_cool(CSpike* b, uint8_t which, uint16_t value); // Set the value on the cool rail

void cspike_set_r(CSpike* b, uint8_t which, uint16_t red_value);
void cspike_set_g(CSpike* b, uint8_t which, uint16_t green_value);
void cspike_set_b(CSpike* b, uint8_t which, uint16_t blue_value);

void cspike_set_rgb(CSpike* b, uint8_t which, uint16_t red_value, uint16_t green_value, uint16_t blue_value);

    

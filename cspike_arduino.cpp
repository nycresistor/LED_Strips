#include "cspike.h"
#include <Arduino.h>



void cspike_int_latch(const CSpike* b) {
    digitalWrite(b->data_pin, LOW);
    delayMicroseconds(240);
    for (int i = 0; i < 4; i++) {
        digitalWrite(b->data_pin, HIGH);
        digitalWrite(b->data_pin, LOW);
    }
    delayMicroseconds(1);
}

void cspike_int_shift16(const CSpike* b, uint16_t data) {
    for (int i = 0; i < 8; i++) {
        digitalWrite(b->data_pin, ((data & 0x8000) == 0)? LOW:HIGH);
        digitalWrite(b->clk_pin, HIGH);
        digitalWrite(b->data_pin, ((data & 0x4000) == 0)? LOW:HIGH);
        digitalWrite(b->clk_pin, LOW);
        data <<= 2;
    }
}

void cspike_init(const CSpike* b) {
    pinMode(b->clk_pin, OUTPUT);
    digitalWrite(b->clk_pin, LOW);
    pinMode(b->data_pin, OUTPUT);
    digitalWrite(b->data_pin, LOW);
    // Immediately load with null data. We send 0s for all the command words. This needs
    // to happen fast so we'll just toggle everything manually.
    // Since we shift on high and low transitions, we can just wiggle the clock line.
    for (int i = 0; i < (CSPIKE_CHIPS + CSPIKE_LEDS)*8; i++) {
        digitalWrite(b->clk_pin, HIGH);
        digitalWrite(b->clk_pin, LOW);
    }
    // Now we'll latch the initialized state.
    cspike_int_latch(b);
};

void cspike_update(const CSpike* b) {
    // Most significant LED shifted in first
    noInterrupts();
    for (int chip = CSPIKE_CHIPS - 1; chip >= 0; chip--) {
        cspike_int_shift16(b, 0x00); // send simplest possible chip command (0, 8-bit mode)
        for (int channel = CSPIKE_CH_PER_CHIP - 1; channel >= 0; channel--) {
          //if (b->leds[(chip*CSPIKE_CH_PER_CHIP)+channel]!=0) Serial.printf("Chip/Channel %d %d\n",chip,channel);
          cspike_int_shift16(b, b->leds[(chip*CSPIKE_CH_PER_CHIP)+channel]);
        }
    }
    cspike_int_latch(b);
    interrupts();
};

void cspike_clear(CSpike* b) {
    memset(b->leds, 0, CSPIKE_LEDS*sizeof(uint16_t));
}

const uint8_t LANE_A_TAB[12] = {
    8,     5,     1,
    12+5,  24+11, 24+7,
    24+1,  36+9,  36+3,
    48+10, 48+5,  48+0,
};

const uint8_t LANE_B_TAB[12] = {
    9,     6,     12+9,
    12+4,  12+0,  24+6,
    24+5,  36+8,  36+7,
    36+2,  48+9,  48+4,
};

const uint8_t LANE_RED_TAB[12] = {
    10,    3,     12+11,
    12+7,  12+2,  24+9,
    24+3,  36+11, 36+5,
    36+0,  48+7,  48+2,
};

const uint8_t LANE_GREEN_TAB[12] = {
    7,     4,     12+10,
    12+8,  12+1,  24+10,
    24+2,  24+0,  36+4,
    36+1,  48+6,  48+3,
};

const uint8_t LANE_BLUE_TAB[12] = {
    11,    2,     0,
    12+6,  12+3,  24+8,
    24+4,  36+10, 36+6,
    48+11, 48+7,  48+1,
};

void cspike_set_lane_A(CSpike* b, uint8_t which, uint16_t value) {
    const uint8_t block = which/12; // There are 12 rows in a repeating block
    b->leds[block*12*5 + LANE_A_TAB[which%12]] = value;
}

void cspike_set_lane_B(CSpike* b, uint8_t which, uint16_t value) {
    const uint8_t block = which/12; // There are 12 rows in a repeating block
    b->leds[block*12*5 + LANE_B_TAB[which%12]] = value;
}

//void cspike_set_hot(CSpike* b, uint8_t which, uint16_t value); // Set the value on the hot rail
//void cspike_set_cool(CSpike* b, uint8_t which, uint16_t value); // Set the value on the cool rail

void cspike_set_r(CSpike* b, uint8_t which, uint16_t red_value) {
    const uint8_t block = which/12; // There are 12 rows in a repeating block
    b->leds[block*12*5 + LANE_RED_TAB[which%12]] = red_value;
}

void cspike_set_g(CSpike* b, uint8_t which, uint16_t green_value) {
    const uint8_t block = which/12; // There are 12 rows in a repeating block
    b->leds[block*12*5 + LANE_GREEN_TAB[which%12]] = green_value;
}

void cspike_set_b(CSpike* b, uint8_t which, uint16_t blue_value) {
    const uint8_t block = which/12; // There are 12 rows in a repeating block
    b->leds[block*12*5 + LANE_BLUE_TAB[which%12]] = blue_value;
}

void cspike_set_rgb(CSpike* b, uint8_t which, uint16_t red_value, uint16_t green_value, uint16_t blue_value) {
    cspike_set_r(b, which, red_value);
    cspike_set_g(b, which, green_value);
    cspike_set_b(b, which, blue_value);
}
    

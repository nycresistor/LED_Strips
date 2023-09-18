#include "ColorSpike_LED.h"
#include <Arduino.h>

/**
 * These are the tables that work out the mapping from the
 * various columns and rows of the LED display to the sequence
 * number of the LED in the chain. This pattern repeats every
 * 5 controller chips, which is to say every 12 rows, so only
 * the first 12 rows are mapped.
 */
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



void ColorSpike_LED::latch() {
    digitalWrite(data_pin, LOW);
    delayMicroseconds(240);
    for (int i = 0; i < 4; i++) {
        digitalWrite(data_pin, HIGH);
        digitalWrite(data_pin, LOW);
    }
    delayMicroseconds(1);
}

void ColorSpike_LED::shift16(uint16_t data) {
    for (int i = 0; i < 8; i++) {
        digitalWrite(data_pin, ((data & 0x8000) == 0)? LOW:HIGH);
        digitalWrite(clk_pin, HIGH);
        digitalWrite(data_pin, ((data & 0x4000) == 0)? LOW:HIGH);
        digitalWrite(clk_pin, LOW);
        data <<= 2;
    }
}

void ColorSpike_LED::init() {
    pinMode(clk_pin, OUTPUT);
    digitalWrite(clk_pin, LOW);
    pinMode(data_pin, OUTPUT);
    digitalWrite(data_pin, LOW);
    // Immediately load with null data. We send 0s for all the command words. This needs
    // to happen fast so we'll just toggle everything manually.
    // Since we shift on high and low transitions, we can just wiggle the clock line.
    for (int i = 0; i < (CSPIKE_CHIPS + CSPIKE_LEDS)*8; i++) {
        digitalWrite(clk_pin, HIGH);
        digitalWrite(clk_pin, LOW);
    }
    // Now we'll latch the initialized state.
    latch();
};

void ColorSpike_LED::update() {
    // Most significant LED shifted in first
    for (int chip = CSPIKE_CHIPS - 1; chip >= 0; chip--) {
        shift16(0x00); // send simplest possible chip command (0, 8-bit mode)
        for (int channel = CSPIKE_CH_PER_CHIP - 1; channel >= 0; channel--) {
          shift16(leds[(chip*CSPIKE_CH_PER_CHIP)+channel]);
        }
    }
    latch();
};

void ColorSpike_LED::clear() {
    memset(leds, 0, CSPIKE_LEDS*sizeof(uint16_t));
}

void ColorSpike_LED::setColA(uint8_t row, uint16_t value) {
    const uint8_t block = row/12; // There are 12 rows in a repeating block
    leds[block*12*5 + LANE_A_TAB[row%12]] = value;
}

void ColorSpike_LED::setColB(uint8_t row, uint16_t value) {
    const uint8_t block = row/12; // There are 12 rows in a repeating block
    leds[block*12*5 + LANE_B_TAB[row%12]] = value;
}

void ColorSpike_LED::setR(uint8_t row, uint16_t value) {
    const uint8_t block = row/12; // There are 12 rows in a repeating block
    leds[block*12*5 + LANE_RED_TAB[row%12]] = value;
}
void ColorSpike_LED::setG(uint8_t row, uint16_t value) {
    const uint8_t block = row/12; // There are 12 rows in a repeating block
    leds[block*12*5 + LANE_GREEN_TAB[row%12]] = value;
}
void ColorSpike_LED::setB(uint8_t row, uint16_t value) {
    const uint8_t block = row/12; // There are 12 rows in a repeating block
    leds[block*12*5 + LANE_BLUE_TAB[row%12]] = value;
}

void ColorSpike_LED::setRGB(uint8_t row, uint16_t red_value, uint16_t green_value, uint16_t blue_value) {
    setR(row, red_value);
    setG(row, green_value);
    setB(row, blue_value);
}
    

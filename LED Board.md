# LED Board

This is a quick document describing the LED strips.

## Overview
Each board carries 25 daisy-chained MY9221 LED drivers, daisy-chained. 

Each board also carries 180 SMD LEDs, organized in 60 banks. Each bank contains one warm white LED, one RGB LED, and one cool white LED. Since that's effectively 5 LEDs per bank, 
we really have 300 LEDs, which comes out to 12 LEDs/driver chip.

## Power
On these boards V<sub>LED</sub> and the driver's VCC are tied together, so the voltage has to remain in the 3V-5.5V range. In the original configuration they appear to have been
running at 5V. 
**IMPORTANT:** Lighting up an entire strip of LEDs can draw a tremendous amount of current. This will cause VCC to sag, leading the driver chips to glitch out, getting the
strip into a bad state. Since the enable and reset lines of the drivers aren't broken out, the only real way to unbrick the strip is to disconnect it and wait for the caps
to discharge. Don't light up too many LEDs at once unless you have a very robust power supply!

## Pinout
There are two connectors attached to the board. One is a 2-pin Molex Mini-Fit Jr. (probably part #0039012020) that connects to power and ground. The other
is a 2mm pitch 2-pin connector carrying clock and data. GND, VCC, SDI and CKI are clearly labelled on the board.

## Driving the MY9221s

### Electrical interface
The data lines require a voltage of 0.7*VDD for V<sub>IH</sub>, so 3.3 signals won't cut it. You'll have to use a microcontroller that produces 5V signals or a buffer chip.

The data is clocked in to the chip on the rising *and* the falling edge of the clock signal. Each chip takes a *208 bit* long command to set its internal registers. That means
to update the entire strip, you'll need to send *5200* bits to the board. After all the data is clocked in to the internal shift register, you'll then have to latch it
to have it take effect. To accomplish this, the chip requires us to:
1. pull the DI line low, leaving the CLK line in whatever state it's currently in
2. wait for 220uS
3. send four pulses on the DI line, leaving the CLK line alone (70nS high, 230nS low)
4. wait 450nS before starting a new operation

Since the fastest we can toggle the CLK line is 50nS, this means a full update cycle takes about 500uS, or half a millisecond. This gives us a theoretical refresh rate of
2000Hz, which is pretty good.

### Protocol

The 208 bits on each chip are clocked in MSB first. The first 16 bits are the command word, followed by 12 16-bit words, one for each LED. 
The chip is configured to use its internal timer, so the simplest way to drive it is to set it in 8-bit mode. In 8-bit mode, you still need to send 16 bits 
for each LEDs, but the high bits should be zero. In this configuration the entire 16-bit command word, which is sent first, is zero. Thus your packet, in terms of bytes, 
looks like this:

| Function | High byte | Low Byte |
|----------|-----------|----------|
| Command  | 0x00      | 0x00     |
| LED[11]  | 0x00      | value    |
| LED[10]  | 0x00      | value    |
| ... |
| LED[0]  | 0x00      | value    |

## LED Map

This table shows which LEDs are associated with each chip and channel.

| Chip | Ch 11 | Ch 10 | Ch 9 | Ch 8 | Ch 7 | Ch 6 | Ch 5 | Ch 4 | Ch 3 | Ch 2 | Ch 1 | Ch 0 |
|------|-------|-------|------|------|------|------|------|------|------|------|------|------|
|    0 |       |       |      |      |      |      |      |      |      |      |      |      |
|    1 |       |       |      |      |      |      |      |      |      |      |      |      |
|    2 |       |       |      |      |      |      |      |      |      |      |      |      |
|    3 |       |       |      |      |      |      |      |      |      |      |      |      |
|    4 |       |       |      |      |      |      |      |      |      |      |      |      |
|    5 |       |       |      |      |      |      |      |      |      |      |      |      |
|    6 |       |       |      |      |      |      |      |      |      |      |      |      |
|    7 |       |       |      |      |      |      |      |      |      |      |      |      |
|    8 |       |       |      |      |      |      |      |      |      |      |      |      |
|    9 |       |       |      |      |      |      |      |      |      |      |      |      |
|   10 |       |       |      |      |      |      |      |      |      |      |      |      |
|   11 |       |       |      |      |      |      |      |      |      |      |      |      |
|   12 |       |       |      |      |      |      |      |      |      |      |      |      |
|   13 |       |       |      |      |      |      |      |      |      |      |      |      |
|   14 |       |       |      |      |      |      |      |      |      |      |      |      |
|   15 |       |       |      |      |      |      |      |      |      |      |      |      |
|   16 |       |       |      |      |      |      |      |      |      |      |      |      |
|   17 |       |       |      |      |      |      |      |      |      |      |      |      |
|   18 |       |       |      |      |      |      |      |      |      |      |      |      |
|   19 |       |       |      |      |      |      |      |      |      |      |      |      |
|   20 |       |       |      |      |      |      |      |      |      |      |      |      |
|   21 |       |       |      |      |      |      |      |      |      |      |      |      |
|   22 |       |       |      |      |      |      |      |      |      |      |      |      |
|   23 |       |       |      |      |      |      |      |      |      |      |      |      |
|   24 |       |       |      |      |      |      |      |      |      |      |      |      |




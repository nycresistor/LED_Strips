# Using the full color LED strip PCBs

## Description

The LED strips discussed here are half-meter long white PCBs each carrying 60 warm white LEDs, 60 cool
white LEDs, and 60 RGB LEDs. They were part of an initial run of stage and screen lighting bars that
were donated to the NYCR by the manufacturer during a move. They have some design shortcomings and a
number of them have assembly defects (paticularly miswired connectors), so it's worth it to double-check
all your connections before you use them.

The boards are rectangular, with a small protruding tab with connectors at one end. They are labelled
"LED_PCB_60_RGB+2W" on the end of the board near the connectors.

There is double-sided adhesive along the back of the boards with a blue plastic protective strip that can be 
peeled away.

## Measurements

The whole board is a hair under half a meter long.

| Measurement            | Value |
|------------------------|-------|
| Overall length         | 498mm |
| Length (body)          | 493mm |
| Width (body)           | 36mm  |
| Length (tab)           | 5mm   |
| Width (tab)            | 18mm  |
| PCB thickness          | 0.8mm |
| PCB+adhesive thickness | 1.6mm |

## LED controllers

The LED controller chips are daisy-chained MY9221s. Data is shifted in using two pins, labelled CKI (clock in)
and SDI (serial data in). Some of the presoldered connectors are wired backwards, so be sure to check the
labels on the board when you connect to them.

The serial protocol used is a bit unusual. Data is shifted in on the rising _and_ falling edges of the clock
signal, so most standard SPI peripherals will not be able to easily control these chips. Additionally, once the
data is shifted in it is "latched" (transferred from the chained shift registers to the control registers) by
holding the clock line steady and toggling the data line. If you're not using the included Arduino library, it's
recommended that you read the datasheet.

### Startup note

The boards are wired so that the enable pin of all the controllers are wired directly to ground. This means that
the MY9221 chips never go through a "start up" state and will simply power up and display random data. It's
importatnt to initialize the MY9221 strips as quickly as possible to avoid flashing a random LED pattern (or
potentially browning out the chips if too many LEDs are lit up). The Arduino library will flush the controllers
with 0s at init time by default.

### MY9221/Interrupt note

The MY9221 is very sensitive to jitter. Currently the Arduino library disables interrupts during transmission
to account for this. You may need to modify the library if this is a problem for your application.

## Powering the board

The board is wired to use the same VCC for both the controller chips and the LEDs. That means you are practically
restricted to the range 3.3V-5V. The boards were designed to run off of 3.7V LiPOs, so this is the recommended
voltage. If you're using a 3.3V microcontroller, you'll need to level shift your serial and clock lines to the
same voltage as the MY9221s (I use a 74HCT244 buffer).

The LEDs will illuminate brightly at 3.3V, but you may want higher voltages if you're planning to use these for
lighting applications.

## Arduino library for controlling the LED board

This repository now includes an Arduino library! The simplest way to install it is to copy the "ColorSpike_LED" directory in the Arduino folder into your local Arduino library folder. It includes a simple example that will exercise the red, green, and blue LEDs. If you're using a 3.3V micrcontroller (which is most of them nowadays) be sure to use some form of 3.3V->5V level shifter.

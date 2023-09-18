# LED Strips
This is just a place to dump info and ideas about the LED strips that arrived at NYCR. Ideally I'd like to resuse the controller boards that came with them.

I've started documenting the controller board. The LED boards are a series of warm white, cool white, and RGB leds driven by daisy-chained MY9221 chips.

## Arduino library for controlling the LED board

This repository now includes an Arduino library! The simplest way to install it is to copy the "ColorSpike_LED" directory in the Arduino folder into your local Arduino library folder. It includes a simple example that will exercise the red, green, and blue LEDs. If you're using a 3.3V micrcontroller (which is most of them nowadays) be sure to use some form of 3.3V->5V level shifter.

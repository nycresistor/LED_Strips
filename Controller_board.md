# LED Strip Controller Board

This board appears to be designed to run off of batteries or DC supply, and potentially charge batteries as well. It includes
a small OLED display and five buttons.

## Labelling
The boards are labelled "PX_BLE_CS_LI_ION 1.3" on the edge near the labelled DC input and battery connections.

## Connectors
The board has five connectors.

At one end of the board we have a two-wire connector labelled "DC" with positive and negative terminals marked. Next to it we have
a battery connector with positive, NTC (temperature sensor), and negative terminals marked.

Midway through the board are a two pin data/clock connector and a two wire 5V/GND connector. These match the connections found on the
LED strip board.

There is a smaller two-pin connector at the end of the board near the display; this may just be meant to provide power to another device.

Finally there is an unpopulated 8-pin header on the reverse of the board near the display; this was probably for programming the microcontroller.

## Chips and modules

### Display
At one end of the board is an unidentified OLED module. It has a labelled 7-pin inline header connection; the pins are labelled (from left to
right, looking at the board) GND VCC D0 D1 RES DC CS. Probably this indicates:

| Label | Function |
|-------|----------|
| GND | Ground |
| VCC | Power (3.3V, most likely) |
| D0 | Data pin 0 |
| D1 | Data pin 1 |
| RES | Reset? |
| DC | Data clock? |
| CS | Chip select |

The protocol is unknown.

### Bluetooth
There is a Raytac MDBT42Q module just below the LED. This is a Nordic nRF52832 based Bluetooth module. It has a 32-bit ARM core onboard and many
of the pins can be used as GPIO. It appears to be driving the OLED display as well, so may be effectively the "brains" of the board.

[Product page](https://www.raytac.com/product/ins.php?index_id=31), [datasheet](https://www.raytac.com/download/index.php?index_id=27)

### LDO buck converter
Around halfway across the board we have a SIC402A, which appears to be [this LDO/buck converter](https://www.vishay.com/en/product/63729/).
Probably for generating the voltages needed by the Raytac module and display.

### Charging
At the other end of the board is a TI [BQ24133](https://www.ti.com/product/BQ24133) battery charging chip. It is surrounded by some upsettingly
large MOSFETs and inductors. The circuit appears to have been at least partially cribbed from the schematic in the BQ24133 datasheet so I'm going
to assume that this is just an overbuilt charging circuit.

## Next steps

Figure out the input voltages, get a board glowing without the LED strip, then move on

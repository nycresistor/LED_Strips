/*!
 * @file ColorSpike_LED.h
 *
 * This is an Arduino library for the excess ColorSpike LED boards that
 * were donated to NYCR. These consist of 60 rows of LEDs, each containing
 * one hot white, one cool white, and one RGB LED, for a total of 300 LEDs/board.
 *
 * The LEDs are driven by a set of 25 daisy-chained MY9221 LED controllers.
 * Because the boards do not break out the reset lines and tie the LED power rails
 * to the controller power rails, the controller will come up in a random state
 * that may put a large load on the power supply. **It is important that the
 * ColorSpike boards be initialized as soon as possible after the microcontroller
 * starts running.** If your microcontroller has any kind of delay on boot (as
 * many Arduinos do), disable it and make sure that the ColorSpike initialization
 * code comes *first*.
 *
 * If you hold the board with the LEDs facing you and the connectors at the bottom,
 * "column A" will be the left-side column of white LEDs, and "column B" will be 
 * on the right.
 * Keep in mind that the "hot" and "cool" white LEDs alternate in each column.
 * 
 * Although all the LED brightness values below are 16-bit, this library currently
 * configures the LED controller to operate in 8-bit brightness mode. The high 8
 * bits of each value should always be zero. (The controller documentation doesn't
 * indicate what happens if high bits are set, but they are probably ignored.)
 */

#ifndef COLORSPIKE_LED_H
#define COLORSPIKE_LED_H

#include <stdint.h>

#define CSPIKE_CHIPS 25         //!< Number of chips on the board
#define CSPIKE_CH_PER_CHIP 12   //!< Number of channels per chip
#define CSPIKE_LENGTH 60        //!< Number of rows of LEDs on the board
#define CSPIKE_LEDS (CSPIKE_CHIPS * CSPIKE_CH_PER_CHIP)  //!< Total number of LEDs on the board

class ColorSpike_LED {

 public:
    /*!
     * Construct the ColorSpike_LED object on the given pins. Be sure to
     * **immediately** call init() on powerup.
     * @param clock Arduino pin number of the clock line
     * @param data Arduino pin number of the data line
     */
    ColorSpike_LED(int clock, int data);
    
    /*!
     * Initialize the LED board. This will turn off all the LEDs.
     */
    void init();

    /*!
     * Update the LEDs with the values in our internal map. (You won't see
     * any changes in the LEDs until you call this function.)
     */
    void update();

    /*! 
     * Clear the internal LED map. This will not affect the display until
     * @ref update() is called.
     */
    void clear();

    /*! @brief Set the LED in column A in the specified row to the given value. 
     * @param row the row of the LED to set
     * @param value an 8-bit brightness value; 255 is maximum brightness and 0 is off. */
    void setColA(uint8_t row, uint16_t value);

    /*! @brief Set the LED in column B in the specified row to the given value.
      @copydetails setColA() */
    void setColB(uint8_t row, uint16_t value);

    /*! @brief Set the red LED in the specified row to the given value.
      @copydetails setColA() */
    void setR(uint8_t row, uint16_t value);
    
    /*! @brief Set the green LED in the specified row to the given value.
      @copydetails setColA() */
    void setG(uint8_t row, uint16_t value);
    
    /*! @brief Set the blue LED in the specified row to the given value.
      @copydetails setColA() */
    void setB(uint8_t row, uint16_t value);

    /*!
     * Set the values of the RGB LED in the given row.
     * @param row the row of the LED to set
     * @param red_value the brightness of the red LED
     * @param green_value the brightness of the green LED
     * @param blue_value the brightness of the blue LED
     */
    void setRGB(uint8_t row, uint16_t red_value, uint16_t green_value, uint16_t blue_value);

    uint16_t leds[300];   //!< LED value map, for those who like direct access
 private:
    void latch(); //!< Latch the data that is currently shifted in to the display
    void shift16(uint16_t data); //!< shift in 16 bits of data MSB-first
    
    int clk_pin;
    int data_pin;
};
    
#endif // COLORSPIKE_LED_H

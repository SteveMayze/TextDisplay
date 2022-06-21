
#ifndef INCLUDE_NEOPIXEL_H
#define INCLUDE_NEOPIXEL_H

#include <avr/io.h>
#include <stdbool.h>
#include "attiny1614_sr595.h"

#define NEO_DENSITY_COMPACT

typedef struct {
	uint8_t pix;
	uint8_t red;
	uint8_t green;
	uint8_t blue;
} pixel_type;


/*! The port pin PA1 */
#define NEOPIXEL_PA1 1
/*! The port pin PA2 */
#define NEOPIXEL_PA2 2
/*! The port pin PA3 */
#define NEOPIXEL_PA3 3
/*! The port pin PA4 */
#define NEOPIXEL_PA4 4
/*! The port pin PA5 */
#define NEOPIXEL_PA5 5
/*! The port pin PA6 */
#define NEOPIXEL_PA6 6
/*! The port pin PA7 */
#define NEOPIXEL_PA7 7


/*! The port for the neopixel and shift register */
#define NEOPIXEL_PORT VPORTA_OUT

/*! The output port pin for driving the NeoPixel strip */
#define NEOPIXEL_NEOPIN SR595_ENABLE

/*! The number of NeoPixels */
#define NEOPIXELS_SIZE 60

/*! The NeoPixel buffer size */
#ifdef NEO_DENSITY_COMPACT
// Compact will have one byte to represent a full colour density.
// These  become an index to a colour array that defines a fixed set
// of colours.

#define NEO_COLOUR_BLACK 0 
#define NEO_COLOUR_RED 1
#define NEO_COLOUR_GREEN 2
#define NEO_COLOUR_BLUE 3

#define NEO_COLOUR_DENSITY 1
#else
// The colour density implies how many bytes are to be used
// in the pixel ram. 3 indicates that each of the RGB will take
// up a byte each.
#define NEO_COLOUR_DENSITY 3
#endif 

#define neopixel_buffer_size NEOPIXELS_SIZE * NEO_COLOUR_DENSITY
/*! The location of the first pixel */
#define FIRST_PIXEL 0
/*! The location of the last pixel */
#define LAST_PIXEL ((NEOPIXELS_SIZE-1) * NEO_COLOUR_DENSITY)
// #define LAST_PIXEL ((NEOPIXELS_SIZE *3) -1 )

/*! The relative location of the RED colour data */
#define NEO_RED 1
/*! The relative location of the GREEN colour data */
#define NEO_GREEN 0
/*! The relative location of the BLUE colour data */
#define NEO_BLUE 2


/*! The highest intensity for a pixel colour i.e. ON*/
#define NEO_ALL_ON ((uint8_t) 0xFF)
/*! The lowest intensity for a pixel colour i.e OFF */
#define NEO_ALL_OFF ((uint8_t) 0x00)

#define NEO_HUE_ADJ 0x0A
#define NEO_HUE_MAX_LIMIT 0xFF - NEO_HUE_ADJ
#define NEO_HUE_MIN_LIMIT NEO_HUE_ADJ

void delay_ms(int ms);

void neopixel_init();

/*!
 * \brief	Sets a pixel with the RGB code
 */
void neopixel_setPixel(uint8_t strip[], uint8_t pixel, uint8_t red, uint8_t green, uint8_t blue);

/*!
 * \brief	Increases the pixel hue to a maximum of 0xFF based on the values contained in the pixel struct.
 */
void neopixel_incPixelHue(uint8_t strip[], pixel_type pixel);

/*!
 * \brief	Increases the pixel hue to a maximum of 0xFF based on the values contained in the pixel struct.
 */
bool neopixel_incPixelHue_with_limit(uint8_t strip[], pixel_type pixel);

/*!
 * \brief	Decreases the pixel hue to zero based on the values contained in the pixel struct.
 */
void neopixel_decrPixelHue(uint8_t strip[], pixel_type pixel);

/*!
 * \brief	Decreases the pixel hue to zero based on the values contained in the pixel struct.
 */
bool neopixel_decrPixelHue_with_limit(uint8_t strip[], pixel_type pixel);

/*!
 * \brief	Shifts the pixels one pixel in the indicated direction
 */
void neopixel_shift(uint8_t strip[], bool direction, bool roll );

/*!
 * \brief	Initialises the buffer with the given colour 
 */
void neopixel_fill(uint8_t strip[], uint8_t red, uint8_t green, uint8_t blue);

uint8_t get_speed();

/*!
 * \brief Pushes the buffer out to the pixel strip.
 */
void neopixel_show(uint8_t strip[]);

void neopixel_setchannel( uint8_t channel );

#endif 


/* 
 * File:   neopixel_colour.h
 * Author: steve
 *
 * Created on June 19, 2022, 3:03 PM
 */

#if !defined(NEOPIXEL_COLOUR_H) && defined(NEO_DESITY_COMPACT)
#define	NEOPIXEL_COLOUR_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <avr/pgmspace.h>


#define NEO_COLOUR_BLACK 0 
#define NEO_COLOUR_RED 1
#define NEO_COLOUR_GREEN 2
#define NEO_COLOUR_BLUE 3
    
const  uint8_t colour_chart[4][3] PROGMEM = { 
    // SP, !, ", #, $, %, &, ', (, ), *, +, ,, -, ., / 
    {0x00, 0x00, 0x00}, // Black
    {0x7F, 0x00, 0x00}, // Red
    {0x00, 0x7F, 0x00}, // Green
    {0x00, 0x00, 0x7F}, // Blue
  
};


#ifdef	__cplusplus
}
#endif

#endif	/* NEOPIXEL_COLOUR_H */


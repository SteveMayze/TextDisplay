/* 
 * File:   neopixel_colour.h
 * Author: steve
 *
 * Created on June 19, 2022, 3:03 PM
 */

#if !defined(NEOPIXEL_COLOUR_H) && defined(NEO_DENSITY_COMPACT)
#define	NEOPIXEL_COLOUR_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <avr/pgmspace.h>
#define RGB_PIXEL
#ifdef RGB_PIXEL
#define COLOUR_COUNT 3
const  uint8_t colour_chart[9][3] PROGMEM = { 
// const  uint8_t colour_chart[4][3] = { 
   // G,   R,    B
    {0x00, 0x00, 0x00}, // 0 Black
    {0x00, 0x7F, 0x00}, // 1 Red
    {0x52, 0x7F, 0x00}, // 2 orange
    {0x7F, 0x7F, 0x00}, // 3 yellow
    {0x7F, 0x00, 0x00}, // 4 Green
    {0x00, 0x00, 0x7F}, // 5 Blue
    {0x00, 0x25, 0x41}, // 6 indigo
    {0x00, 0x40, 0x40}, // 7 Violet
    {0x3F, 0x3F, 0x3F}, // 8 White    
  
};
#endif

#ifdef RGBW_PIXEL
#define COLOUR_COUNT 4
const  uint8_t colour_chart[9][4] PROGMEM = { 
// const  uint8_t colour_chart[4][3] = { 
   // G,   R,    B
    {0x00, 0x00, 0x00, 0x00}, // 0 Black
    {0x00, 0x7F, 0x00, 0x00}, // 1 Red
    {0x52, 0x7F, 0x00, 0x00}, // 2 orange
    {0x7F, 0x7F, 0x00, 0x00}, // 3 yellow
    {0x7F, 0x00, 0x00, 0x00}, // 4 Green
    {0x00, 0x00, 0x7F, 0x00}, // 5 Blue
    {0x00, 0x25, 0x41, 0x00}, // 6 indigo
    {0x00, 0x40, 0x40, 0x00}, // 7 Violet
    {0x00, 0x00, 0x00, 0x7F}, // 8 White    
  
};
#endif

#ifdef	__cplusplus
}
#endif

#endif	/* NEOPIXEL_COLOUR_H */


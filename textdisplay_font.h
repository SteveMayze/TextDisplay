/* 
 * File:   textdisplay_font.h
 * Author: steve
 *
 * Created on June 16, 2022, 10:51 AM
 */

#ifndef TEXTDISPLAY_FONT_H
#define	TEXTDISPLAY_FONT_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <avr/pgmspace.h>


#define FONT_START_CHAR 0x20
// Ensure this goes into the FLASH/Program space rather than RAM. However, this
// will require a special work around to access this data.
    
const  uint8_t font_table[128][5] PROGMEM = { 
    // SP, !, ", #, $, %, &, ', (, ), *, +, ,, -, ., / 
    {0x00, 0x00, 0x00, 0x00, 0x00}, // SP
    {0x00, 0x00, 0xFA, 0x00, 0x00}, // !
    {0x00, 0xE0, 0x00, 0xE0, 0x00},  // "
    {0x28, 0xFE, 0x28, 0xFE, 0x28}, // #
    {0x24, 0x54, 0xFE, 0x54, 0x48}, // $
    {0xC4, 0xC8, 0x10, 0x23, 0x43}, // %
    {0x6C, 0x92, 0xAA, 0x44, 0x0A}, // &
    {0x00, 0xA0, 0xC0, 0x00, 0x00}, // '
    {0x00, 0x38, 0x44, 0x82, 0x00}, // (
    {0x00, 0x82, 0x44, 0x38, 0x00}, // )
    {0x28, 0x10, 0x7C, 0x10, 0x28}, // *
    {0x10, 0x10, 0x7C, 0x10, 0x10}, // +
    {0x00, 0x05, 0x06, 0x00, 0x00}, // ,
    {0x10, 0x10, 0x10, 0x10, 0x10},  // -
    {0x00, 0x06, 0x06, 0x00, 0x00}, // .
    {0x02, 0x04, 0x08, 0x10, 0x20},  // /
    // 0, 1, 2, 3, 4, 5, 6, 7 ,8 9,
    {0x7C, 0x8A, 0x92, 0xA2, 0x7C}, // 0
    {0x00, 0x42, 0xFE, 0x02, 0x00}, // 1
    {0x42, 0x86, 0x8A, 0x92, 0x62},  // 2
    {0x84, 0x82, 0xA2, 0xD2, 0x8C}, // 3
    {0x38, 0x28, 0x48, 0xFE, 0x08}, // 4 
    {0xE4, 0xA2, 0xA2, 0xA2, 0x9C}, // 5
    {0x3C, 0x52, 0x92, 0x92, 0x0C},  // 6
    {0x80, 0x8E, 0x90, 0xA0, 0xC0}, // 7
    {0x6C, 0x92, 0x92, 0x92, 0x6C},  // 8
    {0x60, 0x92, 0x92, 0x94, 0x78}, // 9
    // :, ;, <, =, >, ?, @
     {0x00, 0x36, 0x36, 0x00, 0x00}, // :
     {0x00, 0x35, 0x36, 0x00, 0x00}, // ; 
     {0x10, 0x28, 0x44, 0x82, 0x00}, // < 
     {0x28, 0x28, 0x28, 0x28, 0x28}, // = 
     {0x00, 0x82, 0x44, 0x28, 0x10}, // > 
     {0x40, 0x80, 0x8A, 0x90, 0x60}, // ? 
     {0x4C, 0x92, 0x9E, 0x82, 0x7C}, // @
    // A, B, C, D, E, F
    {0x7E,0x90,0x90,0x90,0x7E}, {0xFE,0x92,0x92,0x92,0x6C}, {0x7C,0x82,0x82,0x82,0x44}, {0xFE,0x82,0x82,0x82,0x7C}, {0xFE,0x92,0x92,0x92,0x82}, {0xFE,0x90,0x90,0x90,0x80},
    // G, H, I, J, K, L
    {0x7C,0x82,0x92,0x92,0x5C}, {0xFE,0x10,0x10,0x10,0xFE}, {0x00,0x82,0xFE,0x82,0x00}, {0x84,0x82,0x82,0x82,0xFC}, {0xFE,0x10,0x28,0x44,0x82}, {0xFE,0x02,0x02,0x02,0x02},
    // M, N, O, P, Q, R
    {0xFE,0x40,0x20,0x40,0xFE}, {0xFE,0x20,0x10,0x08,0xFE}, {0x7C,0x82,0x82,0x82,0x7C}, {0xFE,0x90,0x90,0x90,0x60}, {0x7C,0x82,0x8A,0x86,0x7C}, {0xFE,0x90,0x98,0x94,0x62},
    // S T U V W X
    {0x64,0x92,0x92,0x92,0x4C}, {0x80,0x80,0xFE,0x80,0x80}, {0xFC,0x02,0x02,0x02,0xFC}, {0xF8,0x04,0x02,0x04,0xF8}, {0xF8,0x06,0x38,0x06,0xF8}, {0x82,0x44,0x38,0x44,0x82},
    // Y, Z
    {0xC0,0x30,0x0E,0x30,0xC0}, {0x86,0x8A,0x92,0xA2,0xC2},
    
    //[, \, ], ^, _, `, 
    {0x00, 0x00, 0xFE, 0x82, 0x82},  // [
    {0x20, 0x10, 0x08, 0x04, 0x02}, // / 
    {0x82, 0x82, 0xFE, 0x00, 0x00}, //  ]
    {0x20, 0x40, 0x80, 0x40, 0x20}, // ^ 
    {0x01, 0x01, 0x01, 0x01, 0x01}, // _
    {0x00, 0x80, 0x40, 0x20, 0x00}, // `
    
    // a, b, c, d, e, f
    {0x04,0x2A,0x2A,0x2A,0x1E},{0xFE,0x22,0x22,0x22,0x1C},{0x1C,0x22,0x22,0x22,0x22},{0x1C,0x22,0x22,0x22,0xFE},{0x1C,0x2A,0x2A,0x2A,0x18},{0x00,0x22,0xFE,0xA2,0x00},
    // g, h, i, j, k, l, 
    {0x18,0x25,0x25,0x25,0x3E},{0xFE,0x20,0x20,0x20,0x1E},{0x00,0x22,0xBE,0x02,0x00},{0x00,0x21,0xBE,0x20,0x00},{0xFE,0x08,0x08,0x14,0x22},{0x00,0x82,0xFE,0x02,0x00},
    // m, n, o, p, q, r
    {0x1E,0x10,0x1E,0x10,0x0E},{0x3E,0x20,0x20,0x20,0x1E},{0x1C,0x22,0x22,0x22,0x1C},{0x3F,0x24,0x24,0x24,0x18},{0x18,0x24,0x24,0x24,0x3F},{0x3E,0x10,0x20,0x20,0x00},
    // s, t, u, v, w, x
    {0x12,0x2A,0x2A,0x2A,0x24},{0x00,0x40,0xFC,0x42,0x00},{0x3C,0x02,0x02,0x04,0x3E},{0x38,0x04,0x02,0x04,0x38},{0x38,0x06,0x1C,0x06,0x3C},{0x22,0x14,0x08,0x14,0x22},
    // y, z
    {0x38,0x05,0x05,0x05,0x3E},{0x22,0x26,0x2A,0x32,0x22},
    // {, |, }, ~
    {0x00, 0x00, 0x00, 0x00, 0x00}, // {
    {0x00, 0x00, 0x00, 0x00, 0x00}, //  |
    {0x00, 0x00, 0x00, 0x00, 0x00}, //  }
    {0x00, 0x00, 0x00, 0x00, 0x00}, // ~

    };





#ifdef	__cplusplus
}
#endif

#endif	/* TEXTDISPLAY_FONT_H */


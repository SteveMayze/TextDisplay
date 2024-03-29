/*!
 * \brief The library for driving the NeoPixels for the ATTiny1614
 *
 * \file neopixel.c
 *
 * Created: 22/12/2017 22:07:00
 * \author Steve Mayze
 * \version 1.0
 */ 
#define F_CPU 20000000UL
#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>
#include "avr/interrupt.h"

#include "neopixel.h"
#include "attiny1614_sr595.h"

#ifdef NEO_DENSITY_COMPACT
#include "neopixel_colour.h"
#endif

/*! The buffer that contains all pixel and colour data */
// uint8_t buffer[neopixel_buffer_size];
/*! The pin mask for the output port pin that drives the NeoPixels */
uint8_t pinMask = 0x10; // PA4 NEOPIXEL_ENABLE




/*!
 * \brief a wrapper over the _delay_ms function
 *
 *	This wrapper enables the ms count to be passed in as variable
 *	and not as a integer constant as dictated by the _delay_ms 
 *	function.
 */
void delay_ms(int ms){
	for(int i =0; i < ms; i++){
		_delay_ms(1);
	}
}


void neopixel_init(){
	PORTA.DIR |= (1 << NEOPIXEL_NEOPIN);
    SR595_PORT |= (1 << SR595_PA4 );
	sr595_init();

}


/*!
 * \brief	Sets a pixel with the RGB code
 */
void neopixel_setPixel(uint8_t strip[], uint8_t pixel, uint8_t red, uint8_t green, uint8_t blue)
{
    volatile uint8_t location = pixel * NEO_COLOUR_DENSITY;
#ifndef NEO_DENSITY_COMPACT
	strip[ location + NEO_RED ] = red;
	strip[ location + NEO_GREEN ] = green;
	strip[ location + NEO_BLUE ] = blue;
#else
    strip[ location ] = red; // only one colour is required here.
#endif
}


/*!
 * \brief	Increases the pixel hue to a maximum of 0xFF based on the values contained in the pixel struct.
 */
void neopixel_incPixelHue(uint8_t strip[], pixel_type pixel){
#ifndef NEO_DENSITY_COMPACT
	volatile uint8_t location = pixel.pix * NEO_COLOUR_DENSITY;
	// Don't increase if either or any have reached their ceiling.
	if ( strip[ location + NEO_RED ] <= (0xFF - pixel.red) && 
	     strip[ location + NEO_GREEN ] <= (0xFF - pixel.green) && 
		 strip[ location + NEO_BLUE ] <= (0xFF - pixel.blue)) {
		strip[ location + NEO_RED ] <= (0xFF - pixel.red)? strip[ location + NEO_RED ] += pixel.red: strip[ location + NEO_RED ];
		strip[ location + NEO_GREEN ] <= (0xFF - pixel.green)? strip[ location + NEO_GREEN ] += pixel.green: strip[ location + NEO_GREEN ];
		strip[ location + NEO_BLUE ] <= (0xFF - pixel.blue)? strip[ location + NEO_BLUE ] += pixel.blue: strip[ location + NEO_BLUE ];
	}
#endif
}

bool neopixel_incPixelHue_with_limit(uint8_t strip[], pixel_type pixel){
	bool limit_reached = false;
#ifndef NEO_DENSITY_COMPACT
	volatile uint16_t location = pixel.pix * NEO_COLOUR_DENSITY;
	// Don't increase if either or any have reached their ceiling.
	if ( strip[ location + NEO_RED ] <= (NEO_HUE_MAX_LIMIT - pixel.red) &&
	strip[ location + NEO_GREEN ] <= (NEO_HUE_MAX_LIMIT - pixel.green) &&
	strip[ location + NEO_BLUE ] <= (NEO_HUE_MAX_LIMIT - pixel.blue)) {
		strip[ location + NEO_RED ] = (NEO_HUE_MAX_LIMIT - pixel.red)? (strip[ location + NEO_RED ] += pixel.red): strip[ location + NEO_RED ];
		strip[ location + NEO_GREEN ] = (NEO_HUE_MAX_LIMIT - pixel.green)? (strip[ location + NEO_GREEN ] += pixel.green): strip[ location + NEO_GREEN ];
		strip[ location + NEO_BLUE ] = (NEO_HUE_MAX_LIMIT - pixel.blue)? (strip[ location + NEO_BLUE ] += pixel.blue): strip[ location + NEO_BLUE ];
		} else {
		limit_reached = true;
	}
#endif
	return limit_reached;
}


/*!
 * \brief	Decreases the pixel hue to zero based on the values contained in the pixel struct.
 */
void neopixel_decrPixelHue(uint8_t strip[], pixel_type pixel){
#ifndef NEO_DENSITY_COMPACT
	volatile uint8_t location = pixel.pix * NEO_COLOUR_DENSITY;
	if( strip[ location + NEO_RED ] > 0 ) {
		strip[ location + NEO_RED ] >= (0x00 + pixel.red)? strip[ location + NEO_RED ] -= pixel.red: strip[ location + NEO_RED ];
	}

	if( strip[ location + NEO_GREEN ] > 0 ) {
		strip[ location + NEO_GREEN ] >= (0x00 + pixel.green)? strip[ location + NEO_GREEN ] -= pixel.green: strip[ location + NEO_GREEN ];
	}

	if( strip[ location + NEO_BLUE ] > 0 ) {
		strip[ location + NEO_BLUE ] >= (0x00 - pixel.blue)? strip[ location + NEO_BLUE ] -= pixel.blue: strip[ location + NEO_BLUE ];
	}
#endif
}

/*!
 * \brief	Decreases the pixel hue to zero based on the values contained in the pixel struct.
 */
bool neopixel_decrPixelHue_with_limit(uint8_t strip[], pixel_type pixel) {
	bool limit_reached = false;
#ifndef NEO_DENSITY_COMPACT
	volatile uint16_t location = pixel.pix * NEO_COLOUR_DENSITY;
	if( strip[ location + NEO_RED ] > NEO_HUE_MIN_LIMIT ) {
		strip[ location + NEO_RED ] = ( strip[ location + NEO_RED ] > NEO_HUE_MIN_LIMIT) ? (strip[ location + NEO_RED ] -= pixel.red): 0x00;
	} 

	if( strip[ location + NEO_GREEN ] > NEO_HUE_MIN_LIMIT ) {
		strip[ location + NEO_GREEN ] = (strip[ location + NEO_GREEN ] > NEO_HUE_MIN_LIMIT) ? (strip[ location + NEO_GREEN ] -= pixel.green): 0x00;
	}

	if( strip[ location + NEO_BLUE ] > NEO_HUE_MIN_LIMIT ) {
		strip[ location + NEO_BLUE ] = ( strip[ location + NEO_BLUE ] > NEO_HUE_MIN_LIMIT) ? (strip[ location + NEO_BLUE ] -= pixel.blue): 0x00;
	}


	if ( strip[ location + NEO_RED ] <= NEO_HUE_MIN_LIMIT && strip[ location + NEO_GREEN ] <=  NEO_HUE_MIN_LIMIT && strip[ location + NEO_BLUE ] <= NEO_HUE_MIN_LIMIT){
		limit_reached = true;
	}
#endif
	return limit_reached;
}

/*!
 * \brief	Initialises the buffer with the given colour 
 */
void neopixel_fill(uint8_t strip[], uint8_t red, uint8_t green, uint8_t blue){
   for(int i = 0; i < NEOPIXELS_SIZE; i++)
   {
      neopixel_setPixel(strip, i, red, green, blue);
   }
}

/*!
 * \brief	Shifts the pixels one pixel in the indicated direction
 */
void neopixel_shift_dense(uint8_t strip[], bool direction, bool roll){

	uint8_t tmp_red, tmp_green, tmp_blue;

   if( direction ) {
		tmp_red = strip[ LAST_PIXEL + NEO_RED];
		tmp_green =  strip[ LAST_PIXEL + NEO_GREEN];
		tmp_blue =  strip[ LAST_PIXEL + NEO_BLUE];

   	    for (int i = NEOPIXELS_SIZE-1; i > 0; i--){
	   	    uint8_t  baseLocation= i * NEO_COLOUR_DENSITY;
	   	    uint8_t newBaseLocation = ( i - 1) * NEO_COLOUR_DENSITY;

	   	    strip[ baseLocation + NEO_RED ] = strip[ newBaseLocation + NEO_RED ];
	   	    strip[ baseLocation + NEO_GREEN ] = strip[newBaseLocation + NEO_GREEN];
	   	    strip[ baseLocation + NEO_BLUE ] = strip[newBaseLocation + NEO_BLUE];
   	    }
        if ( roll ){
            strip[NEO_RED] = tmp_red;
            strip[NEO_GREEN] = tmp_green;
            strip[NEO_BLUE] = tmp_blue;
        }
	} else {

		tmp_red = strip[  NEO_RED ];
		tmp_green =  strip[  NEO_GREEN ];
		tmp_blue =  strip[  NEO_BLUE ];
		for (int i = 0; i < ( NEOPIXELS_SIZE-1 ); i++){
			uint8_t baseLocation = i * NEO_COLOUR_DENSITY;
			uint8_t newBaseLocation = (i + 1) * NEO_COLOUR_DENSITY;

			strip[ baseLocation + NEO_RED ] = strip[ newBaseLocation + NEO_RED ];
			strip[ baseLocation + NEO_GREEN ] = strip[newBaseLocation + NEO_GREEN];
			strip[ baseLocation +NEO_BLUE ] = strip[newBaseLocation + NEO_BLUE];
		}
        if ( roll ) {
            strip[LAST_PIXEL + NEO_RED] = tmp_red;
            strip[LAST_PIXEL + NEO_GREEN] = tmp_green;
            strip[LAST_PIXEL + NEO_BLUE] = tmp_blue;
        }
	}
}

void neopixel_shift_compact(uint8_t strip[], bool direction, bool roll){

	uint8_t tmp_pix;

   if( direction ) {
		tmp_pix = strip[ LAST_PIXEL];

   	    for (int i = NEOPIXELS_SIZE-1; i > 0; i--){
	   	    uint8_t  baseLocation= i;
	   	    uint8_t newBaseLocation = ( i - 1);

	   	    strip[ baseLocation  ] = strip[ newBaseLocation  ];
   	    }
        if ( roll ){
            strip[0] = tmp_pix;
        }
	} else {

		tmp_pix = strip[0];
		for (int i = 0; i < ( NEOPIXELS_SIZE-1 ); i++){
			uint8_t baseLocation = i;
			uint8_t newBaseLocation = (i + 1);

			strip[ baseLocation ] = strip[ newBaseLocation ];
		}
        if ( roll ) {
            strip[LAST_PIXEL] = tmp_pix;
        }
	}
}


void neopixel_shift(uint8_t strip[], bool direction, bool roll) {
#ifndef NEO_DENSITY_COMPACT
    neopixel_shift_dense(strip, direction, roll);
#else
    neopixel_shift_compact(strip, direction, roll);
#endif
}

/*!
 * \brief Pushes the buffer out to the pixel strip.
 */
#ifdef NEO_DENSITY_COMPACT

void neopixel_show(uint8_t strip[])
{
    uint8_t pdata;
    for (uint8_t p = 0; p < neopixel_buffer_size; p++) {
        // Grab the colour definition from the colour_chart based 
        // on the colour specification for the pixel.
        uint8_t colour_idx = strip[p];
        cli();
        for(uint8_t i = 0; i<COLOUR_COUNT; i++){
            pdata =  pgm_read_byte( &(colour_chart[colour_idx][i]));  
            // pdata =  colour_chart[colour_idx][i];  
            //SR595_PORT |= (1 << SR595_PA4 );
            for( uint8_t i=0; i<8; i++){
                if( pdata & 0b10000000 ) { // HIGH = 0.6, 0.6 uS
                    SR595_PORT &= (~(1 << SR595_PA4 ));
                    _delay_us(0.6);
                    SR595_PORT |= (1 << SR595_PA4 );
                    _delay_us(0.6);
                } else { // LOW = 0.3, 0.9 uS
                    //cli();
                    SR595_PORT &= (~(1 << SR595_PA4 ));
                    _delay_us(0.3);
                    SR595_PORT |= (1 << SR595_PA4 );
                    _delay_us(0.9);
                    //sei(); 

                }
                pdata = pdata << 1;
            }
        }
        SR595_PORT |= (1 << SR595_PA4 );
        sei();
    }
}

#else
void neopixel_show(uint8_t strip[])
{
	volatile uint16_t  i = neopixel_buffer_size; // Loop counter

	volatile uint8_t *port;

	volatile uint8_t *ptr = &strip[0],   // Pointer to next byte
	b  = *ptr++,    // Current byte value
	hi,             // PORT w/output bit set high
	lo;             // PORT w/output bit set low

	volatile uint8_t bit;

	lo = VPORTA_OUT |  pinMask;
	hi = VPORTA_OUT & ~pinMask;
    bit  = 8;

	port = &VPORTA_OUT;

	VPORTA_OUT = lo;

    asm volatile(
    "neo_start:"						"\n\t"	//	Tick	Pseudo code	
		"st %a[port], %[hi]"			"\n\t"	//	2		PORT = Hi
	    "sbrc %[byte],  7"				"\n\t"	//	1-3		Skip the next operation if bit 7 is clear
		"rjmp do_HI"					"\n\t"	//	0-2		Jump and Do the HI case

	"do_LOW:"							"\n\t"	//			At 3 cycles. For LOW, still need 3 for HI and then 18 LOW
		"nop"							"\n\t"	//	1
		"nop"							"\n\t"	//	1
		"st %a[port], %[lo]"			"\n\t"	//	2		PORT = Low (Finish of 6 Hi cycles- start the 18 LOW)
		"nop"							"\n\t"	//	1
		"nop"							"\n\t"	//	1
		"nop"							"\n\t"	//	1
		"nop"							"\n\t"	//	1
		"nop"							"\n\t"	//	1
		"nop"							"\n\t"	//	1
		"nop"							"\n\t"	//	1
		"nop"							"\n\t"	//	1
		"nop"							"\n\t"	//	1
		"nop"							"\n\t"	//	1
		"rjmp next_bit"					"\n\t"	//	2

	"do_HI:"							"\n\t"	//			At 3 cycles. For HI, still need 9 for HI and then 12 LOW
		"nop"							"\n\t"	//	1
		"nop"							"\n\t"	//	1
		"nop"							"\n\t"	//	1
		"nop"							"\n\t"	//	1
		"nop"							"\n\t"	//	1
		"nop"							"\n\t"	//	1
		"nop"							"\n\t"	//	1
		"nop"							"\n\t"	//	1
		"st %a[port], %[lo]"			"\n\t"	//	2		PORT = Low (Finish of 16 Hi cycles- start the 9 LOW)
		 "nop"							"\n\t"	//	1
		 "nop"							"\n\t"	//	1
		 "nop"							"\n\t"	//	1
		"rjmp next_bit"					"\n\t"	//	2
		 "nop"							"\n\t"	//	1
	"next_bit:"							"\n\t"	//			This section costs 3 - 5 cycles
		"dec  %[bit]"					"\n\t"	//	1		bit--
		"breq next_byte"				"\n\t"	//	1-2		branch if(bit == 0) (from dec above)
		"rol  %[byte]"					"\n\t"	//	1		b <<= 1 - Roll the byte left one bit.
		"rjmp neo_start"				"\n\t"	//	2		Jump to start for the next bit

	"next_byte:"						"\n\t"	//			This section costs 9 cycles!
		"ldi %[bit], 8"					"\n\t"	// 1		Set bit = 8
		"ld %[byte], %a[ptr]+"			"\n\t"	// 2		Set b = *ptr++
		"st %a[port], %[lo]"			"\n\t"	// 2		PORT = lo
		"sbiw %[count], 1"				"\n\t"	// 2		i--
		"brne neo_start"				"\n"	// 2		if(i != 0) -> (next byte)
    : [port]  "+e" (port),
      [byte]  "+r" (b),
      [bit]   "+r" (bit),
      [count] "+w" (i)
    : [ptr]    "e" (ptr),
      [hi]     "r" (hi),
      [lo]     "r" (lo));

}
#endif

 void neopixel_setchannel( uint8_t channel ) {
	sr595_shiftout( channel );
 }


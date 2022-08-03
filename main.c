/*
 * NeoStrip5Channel_plus.c
 *
 * Created: 11/06/2022 09:46:43
 * Author : Steven
 */ 


#include "time.h"
#include "avr/interrupt.h"
#include "neopixel.h"
#include "textdisplay.h"
#include "usart0.h"

/*! The setting for the CLKCTRL.MCLKCTRLB register */
#define _MAIN_CLOCK 0x00

/************************************************************************/
/* The main control loop                                                */
/************************************************************************/
int main(void)
{

	// This register is protected and can not be changed until the CPP register in the CPU
	// is written with the signature of 0xD8 ... I think I need to insert some assembly code here
	// This PEN flag is reset, this means that the Source clock is fed right through and not pre-scaled.
	// This needs to be checked for the electrical characteristics is such that the full 5V is required.
	// uint8_t strips[NEOPIXELS_SIZE *3];

	CPU_CCP = CCP_IOREG_gc;
	CLKCTRL.MCLKCTRLB = _MAIN_CLOCK;

	neopixel_init();
    
    PORTB.DIRSET |= (1 << 2); // PB2 TxD, PB3 RxD
    sei(); 
    USART0_Initialize();
    
    textdisplay_initialise();
    
	while(true) {
        textdisplay_ctrl();
        textdisplay_roll_text();
        delay_ms(75);
	}
	return 0;
}


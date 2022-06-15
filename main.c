/*
 * NeoStrip5Channel_plus.c
 *
 * Created: 11/06/2022 09:46:43
 * Author : Steven
 */ 

#include "stdlib.h"
#include "time.h"
#include <avr/io.h>
#include "avr/interrupt.h"

#include "neopixel.h"
#include "usart0.h"
#include "stdio.h"
#include "string.h"

#include <avr/pgmspace.h>

/*! The setting for the CLKCTRL.MCLKCTRLB register */
#define _MAIN_CLOCK 0x00

#define BASE_HUE ((uint8_t) 0x03)
#define MED_HUE  ((uint8_t) 0x06)
#define HI_HUE   ((uint8_t) 0x0C)
#define MAX_BUFFERS 8
#define NEO_ANIM_CYCLES 100 // NEO_HUE_ADJ * 5
#define NEO_ANIM_MAX_GRADIENT 100

#define DEFAULT_RED HI_HUE
#define DEFAULT_GREEN 0x00
#define DEFAULT_BLUE 0x00

// Ensure this goes into the FLASH/Program space rather than RAM. However, this
// will require a special work around to access this data.
const  uint8_t font_table[128][5] PROGMEM = { 
    // SP, !, ", #, $, %, &, ', (, ), *, +, ,, -, ., / 
    {0x00, 0x00, 0x00, 0x00, 0x00}, // SP
    {0x00,0x00,0xFA,0x00,0x00}, // !
    {0x00,0xE0,0x00,0xE0,0x00},  // "
    {0x28,0xFE,0x28,0xFE,0x28}, // #
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
    {0x00, 0x41, 0xFF, 0x01, 0x00}, // 1
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

#define DEFAULT_MESSAGE "ZAM PCS Texteck "
uint8_t display_buffer[MAX_BUFFERS][neopixel_buffer_size];

char message[255] = DEFAULT_MESSAGE;
uint8_t message_length = 16;
uint8_t chr_idx = 0;
uint8_t column = 0;
uint8_t data = 0;
uint8_t checksum = 0x00;

uint8_t colour_red = DEFAULT_RED;
uint8_t colour_green = DEFAULT_GREEN;
uint8_t colour_blue = DEFAULT_BLUE;

pixel_type message_buffer[MAX_BUFFERS][5]; // The number of characters in the string x 5 for each char and x3 for each LED.




uint8_t clear_message_buffer(pixel_type message_buffer[][5]) {
    for (uint8_t col=0; col < 5; col++){
        for(uint8_t row = 0; row < MAX_BUFFERS; row++){
            message_buffer[row][col].red = 0x00; 
            message_buffer[row][col].green = 0x00; 
            message_buffer[row][col].blue = 0x00; 
        }
    }
    return 0;
}

uint8_t render_next_char(char *message, uint8_t message_length, uint8_t char_idx,
        uint8_t red, uint8_t green, uint8_t blue,
        pixel_type message_buffer[][5]){
    if (message[char_idx] == ' ') {
        clear_message_buffer(message_buffer);
        return 0;  
    } 
    uint8_t font_idx = (message[char_idx] & 0x7F) - ' ';
    // The font information is stored in the programs space and needs
    // this workaround to access it.
    uint8_t font_char[5];
    for (uint8_t i=0; i<5;i++){
        font_char[i] =  pgm_read_byte( &(font_table[font_idx][i]));        
    }
    for(uint8_t col = 0; col < 5; col++){
        for(uint8_t row = 0; row < MAX_BUFFERS; row++){
            if( font_char[col] & (1<<row)) {
                message_buffer[row][col].red = red;
                message_buffer[row][col].green = green;
                message_buffer[row][col].blue = blue;
            }
        }
    }
    return 0;
}

void roll_text(){
    if ( column > 4 ){
        for( uint8_t channel = 0; channel < MAX_BUFFERS; channel++){
            neopixel_shift(display_buffer[channel], true, false);
            display_buffer[channel][NEO_RED] = 0x00;
            display_buffer[channel][NEO_GREEN] = 0x00;
            display_buffer[channel][NEO_BLUE] = 0x00;
            neopixel_setchannel(1<<channel);
            neopixel_show(display_buffer[channel]);
        }
        column = 0;
        chr_idx++;
        if ( chr_idx > message_length -1){
            chr_idx = 0;
        }
        clear_message_buffer(message_buffer);
        render_next_char(message, message_length, chr_idx, 
                colour_red, colour_green, colour_blue, message_buffer);
    } else {
        for( uint8_t channel = 0; channel < MAX_BUFFERS; channel++){
            neopixel_shift(display_buffer[channel], true, false);
            display_buffer[channel][NEO_RED] = message_buffer[channel][column].red;
            display_buffer[channel][NEO_GREEN] = message_buffer[channel][column].green;
            display_buffer[channel][NEO_BLUE] = message_buffer[channel][column].blue;
            neopixel_setchannel(1<<channel);
            neopixel_show(display_buffer[channel]);
        }
        column++;
    }
    
}

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
    
    clear_message_buffer(message_buffer);
    render_next_char(message, message_length, chr_idx, 
            colour_red, colour_green, colour_blue, message_buffer);
    
    // Flush the serial port
    while(USART0_IsRxReady())
        data = USART0_Read();
    
	while(true) {
        if(USART0_IsRxReady()) {
            while(USART0_IsRxReady()){
                data = USART0_Read();
                if( data == 0xFE ){ // Frame Delimiter
                   uint8_t data_length = USART0_Read();
                   
                   uint8_t action = USART0_Read();
                   if( action == 0x01 ){ // MESSAGE
                       checksum = 0x01;
                        for (uint8_t i = 0; i < data_length-1; i++){
                            message[i] = USART0_Read();
                            checksum += message[i];
                        }
                        message[data_length-1] = 0x00;
                        message_length = data_length-1;
                   } else if ( action == 0x02 ) {
                       // Colour
                       checksum = 0x02;
                       colour_red = USART0_Read();
                       colour_green = USART0_Read();
                       colour_blue = USART0_Read();
                       checksum += colour_red;
                       checksum += colour_green;
                       checksum += colour_blue;
                   } else if (action == 0x03 ){
                       // Reset
                       strcpy(message, DEFAULT_MESSAGE);
                       message_length = strlen(DEFAULT_MESSAGE);
                   } else {
                       strcpy(message, DEFAULT_MESSAGE);
                       message_length = strlen(message);
                       column = 0;
                       chr_idx = 0;
                       USART0_Write(0xFE);
                       USART0_Write(0x02);
                       USART0_Write(0xAC);
                       USART0_Write(action);
                       USART0_Write(0x01);
                       break;                       
                   }
                   checksum &= 0xFF;
                   checksum = 0xFF - checksum;
                   data = USART0_Read();
                   if( checksum == data ){
                       USART0_Write(0xFE);
                       USART0_Write(0x01);
                       USART0_Write(0x01);
                   } else {
                       strcpy(message, DEFAULT_MESSAGE);
                       message_length = strlen(message);
                       column = 0;
                       chr_idx = 0;
                       USART0_Write(0xFE);
                       USART0_Write(0x02);
                       USART0_Write(0xAA);
                       USART0_Write(checksum);
                       USART0_Write(0x01);
                       break;
                   }
                } else {
                       strcpy(message, DEFAULT_MESSAGE);
                       message_length = strlen(message);
                       column = 0;
                       chr_idx = 0;
                       USART0_Write(0xFE);
                       USART0_Write(0x02);
                       USART0_Write(0xAB);
                       USART0_Write(0x00);
                       USART0_Write(0x01);
                       break;
                   }
            }
            clear_message_buffer(message_buffer);
            render_next_char(message, message_length, chr_idx, 
                    colour_red, colour_green, colour_blue, message_buffer);
            for(uint8_t channel = 0; channel < MAX_BUFFERS; channel++){
                neopixel_fill(display_buffer[channel],0x00,0x00,0x00);
                neopixel_show(display_buffer[channel]);
            }
        }
        roll_text();
        delay_ms(100);
        
	}
	return 0;
}


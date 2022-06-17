
#include "textdisplay.h"
#include "neopixel.h"
#include "textdisplay_font.h"
#include "usart0.h"
#include "string.h"

#define DEFAULT_RED HI_HUE
#define DEFAULT_GREEN 0x00
#define DEFAULT_BLUE 0x00

#define BASE_HUE ((uint8_t) 0x03)
#define MED_HUE  ((uint8_t) 0x06)
#define HI_HUE   ((uint8_t) 0x0C)

#define MAX_BUFFERS 8

#define DEFAULT_MESSAGE "ZAM PCS Texteck "
uint8_t display_buffer[MAX_BUFFERS][neopixel_buffer_size];

char message[128] = DEFAULT_MESSAGE;
uint8_t message_length = 16;
uint8_t chr_idx = 0;
uint8_t column = 0;
uint8_t data = 0;
uint8_t checksum = 0x00;


uint8_t colour_red = DEFAULT_RED;
uint8_t colour_green = DEFAULT_GREEN;
uint8_t colour_blue = DEFAULT_BLUE;

#define FRAME_DELIMITER 0xFE
#define ACTION_MESSAGE 0x01
#define ACTION_COLOUR 0x02
#define ACTION_SPEED 0x03
#define ACTION_RESET 0x04
#define INVALID_HEADER 0x81
#define INVALID_ACTION 0x82
#define BAD_CHKSUM 0x83

pixel_type message_buffer[MAX_BUFFERS][5]; // The number of characters in the string x 5 for each char and x3 for each LED.

typedef struct _Data_Frame {
  // uint8_t data_length;
  uint8_t action;
  uint8_t data[64];
  // uint8_t checksum;
  
} Data_Frame;

typedef struct _Messasge_Frame {
    uint8_t frame_header; // 0xFE;
    uint8_t data_length;
    Data_Frame data_frame;
    uint8_t checksum;
} Message_Frame_Type;

typedef union {
    Message_Frame_Type item;
    uint8_t raw[68];
} Message_Frame;


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


uint8_t render_next_char(char message[], uint8_t message_length, uint8_t char_idx,
        uint8_t red, uint8_t green, uint8_t blue,
        pixel_type message_buffer[][5]){
    if (message[char_idx] == ' ') {
        clear_message_buffer(message_buffer);
        return 0;  
    } 
    uint8_t font_idx = (message[char_idx] & 0x7F) - FONT_START_CHAR;
    // The font information is stored in the programs space and needs
    // this workaround to access it.
    uint8_t font_char[5];
    for (uint8_t i=0; i<5; i++){
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

void send_response(Message_Frame *message_frame) {
    uint8_t checksum = 0;
    for (uint8_t i = 2; i<message_frame->item.data_length; i++){
        checksum += message_frame->raw[i];
    }
    checksum &= 0xFF;
    message_frame->item.checksum = 0xFF - checksum;
    for(uint8_t i = 0; i < message_frame->item.data_length+3; i++){
        USART0_Write(message_frame->raw[i]);
    }
}

void textdisplay_initialise(){

    clear_message_buffer(message_buffer);
    render_next_char(message, message_length, chr_idx, 
            colour_red, colour_green, colour_blue, message_buffer);
    USART0_flush();

}


void textdisplay_roll_text(){
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



void textdisplay_ctrl(){
    if(USART0_IsRxReady()) {
        Message_Frame message_frame;
        message_frame.item.frame_header = USART0_Read();
        if( message_frame.item.frame_header == FRAME_DELIMITER ){ // Frame Delimiter
           message_frame.item.data_length = USART0_Read();

           message_frame.item.data_frame.action = USART0_Read();
           checksum = message_frame.item.data_frame.action;
           for (uint8_t i = 0; i < message_frame.item.data_length-1; i++){
               message_frame.item.data_frame.data[i] = USART0_Read();
               checksum += message_frame.item.data_frame.data[i];
           }
           message_frame.item.data_frame.data[message_frame.item.data_length-1] = 0x00;
           checksum &= 0xFF;
           checksum = 0xFF - checksum;
           message_frame.item.checksum = USART0_Read();
           if( checksum == message_frame.item.checksum ){
               USART0_Write(FRAME_DELIMITER);
               USART0_Write(0x01);
               USART0_Write(0x01);
               USART0_Write(0x01);
           } else { // Bad checksum
               strcpy(message, DEFAULT_MESSAGE);
               message_length = strlen(message);
               column = 0;
               chr_idx = 0;
               message_frame.item.frame_header = FRAME_DELIMITER;
               message_frame.item.data_length = 0x02;
               message_frame.item.data_frame.action = BAD_CHKSUM;
               message_frame.item.data_frame.data[0] = checksum;
               send_response(&message_frame);
           }

           if( message_frame.item.data_frame.action == ACTION_MESSAGE ){ // MESSAGE
               strcpy(message, (char*)message_frame.item.data_frame.data);
                message_length = strlen(message);
                chr_idx = 0;
                // TODO Clear the display.
                clear_message_buffer(message_buffer);
                render_next_char(message, message_length, chr_idx, 
                        colour_red, colour_green, colour_blue, message_buffer);
                for(uint8_t channel = 0; channel < MAX_BUFFERS; channel++){
                    neopixel_fill(display_buffer[channel],0x00,0x00,0x00);
                    neopixel_setchannel(1<<channel);
                    neopixel_show(display_buffer[channel]);
                }
           } else if ( message_frame.item.data_frame.action == ACTION_COLOUR ) {
               // Colour
               colour_red = message_frame.item.data_frame.data[0];
               colour_green = message_frame.item.data_frame.data[1];
               colour_blue = message_frame.item.data_frame.data[2];
           } else if (message_frame.item.data_frame.action == ACTION_RESET ){
               // Reset
               strcpy(message, DEFAULT_MESSAGE);
               message_length = strlen(DEFAULT_MESSAGE);
               chr_idx = 0;
                clear_message_buffer(message_buffer);
                render_next_char(message, message_length, chr_idx, 
                        colour_red, colour_green, colour_blue, message_buffer);
                for(uint8_t channel = 0; channel < MAX_BUFFERS; channel++){
                    neopixel_fill(display_buffer[channel],0x00,0x00,0x00);
                    neopixel_setchannel(1<<channel);
                    neopixel_show(display_buffer[channel]);
                }
           } else { // Invalid action
               strcpy(message, DEFAULT_MESSAGE);
               message_length = strlen(message);
               column = 0;
               chr_idx = 0;
               message_frame.item.frame_header = FRAME_DELIMITER;
               message_frame.item.data_length = 0x02;
               message_frame.item.data_frame.action = INVALID_ACTION;
               message_frame.item.data_frame.data[0] = message_frame.item.data_frame.action;
               send_response(&message_frame);
               clear_message_buffer(message_buffer);
               render_next_char(message, message_length, chr_idx, 
                        colour_red, colour_green, colour_blue, message_buffer);
                for(uint8_t channel = 0; channel < MAX_BUFFERS; channel++){
                    neopixel_fill(display_buffer[channel],0x00,0x00,0x00);
                    neopixel_setchannel(1<<channel);
                    neopixel_show(display_buffer[channel]);
                }
           }
        } else { // Invalid Header
            strcpy(message, DEFAULT_MESSAGE);
            message_length = strlen(message);
            column = 0;
            chr_idx = 0;
            message_frame.item.frame_header = FRAME_DELIMITER;
            message_frame.item.data_length = 0x01;
            message_frame.item.data_frame.action = INVALID_HEADER;
            message_frame.item.data_frame.data[0] = message_frame.item.data_frame.action;
            send_response(&message_frame);
            USART0_flush();
            clear_message_buffer(message_buffer);
             render_next_char(message, message_length, chr_idx, 
                     colour_red, colour_green, colour_blue, message_buffer);
             for(uint8_t channel = 0; channel < MAX_BUFFERS; channel++){
                neopixel_fill(display_buffer[channel],0x00,0x00,0x00);
                neopixel_setchannel(1<<channel);
                neopixel_show(display_buffer[channel]);
             }
        }
    }
}

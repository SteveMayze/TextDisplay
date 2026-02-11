# Text Display
This project is an extenion on the NeoPixal driver that reconfigures the NewPixel
driver to a moving text display. This project still utilises the ATtiny20

# Features
- Uses a fixed protocol over the USART
- The text colour can be changed

# Protocol

| 0               | 1              | 2      | 3..n | n+1       |
|-----------------|----------------|--------|------|-----------|
| FRAME DELIMITER | Message length | Action | Data | Check sum |
| 0xFE              | n              |        |      |           |


*Actions*

|ACTION_MESSAGE| 0x01|
|:----|:----|
|ACTION_COLOUR| 0x02|
|ACTION_SPEED| 0x03|
|ACTION_RESET| 0x04|
|ACTION_DEBUG| 0x05|
|INVALID_HEADER| 0x81|
|INVALID_ACTION| 0x82|
|BAD_CHKSUM| 0x83|
|MESSAGE_TOO_LONG| 0x84|

## ACTION_COLOUR

| 0x02| Foreground | Background |

## ACTION_SPEED

| 0x03| Speed |

## ACTION_RESET

|0x04|

## INVALID_ACTION

| 0x82| Action |

## BAD_CHKSUM

| 0x83| checksum |

## MESSAGE_TOO_LONG

| 0x84| Frame length | Max frame length |



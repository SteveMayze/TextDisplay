/* 
 * File:   usart.h
 * Author: steve
 *
 * Created on June 12, 2022, 7:11 PM
 */

#ifndef USART_H
#define	USART_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <avr/io.h>
#include <stdbool.h>
#include "avr/interrupt.h"
#include "util/atomic.h"


/* Normal Mode, Baud register value */
#define USART0_BAUD_RATE(BAUD_RATE) (((float)20000000 * 64 / (16 * (float)BAUD_RATE)) + 0.5)

/* USART0 Ringbuffer */

#define USART0_RX_BUFFER_SIZE 128
#define USART0_TX_BUFFER_SIZE 8
#define USART0_RX_BUFFER_MASK (USART0_RX_BUFFER_SIZE - 1)
#define USART0_TX_BUFFER_MASK (USART0_TX_BUFFER_SIZE - 1)

typedef enum { USART0_RX_CB = 1, USART0_TX_CB } usart0_cb_t;
typedef void (*usart_callback)(void);

/**
 * \brief Initialize USART interface
 * If module is configured to disabled state, the clock to the USART is disabled
 * if this is supported by the device's clock system.
 *
 * \return Initialization status.
 * \retval 0 the USART init was successful
 * \retval 1 the USART init was not successful
 */
void USART0_Initialize();

/**
 * \brief Enable RX and TX in USART0
 * 1. If supported by the clock system, enables the clock to the USART
 * 2. Enables the USART module by setting the RX and TX enable-bits in the USART control register
 *
 * \return Nothing
 */
void USART0_Enable();

/**
 * \brief Enable RX in USART0
 * 1. If supported by the clock system, enables the clock to the USART
 * 2. Enables the USART module by setting the RX enable-bit in the USART control register
 *
 * \return Nothing
 */
void USART0_EnableRx();

/**
 * \brief Enable TX in USART0
 * 1. If supported by the clock system, enables the clock to the USART
 * 2. Enables the USART module by setting the TX enable-bit in the USART control register
 *
 * \return Nothing
 */
void USART0_EnableTx();

/**
 * \brief Disable USART0
 * 1. Disables the USART module by clearing the enable-bit(s) in the USART control register
 * 2. If supported by the clock system, disables the clock to the USART
 *
 * \return Nothing
 */
void USART0_Disable();

/**
 * \brief Get recieved data from USART0
 *
 * \return Data register from USART0 module
 */
uint8_t USART0_GetData();

/**
 * \brief Check if the usart can accept data to be transmitted
 *
 * \return The status of USART TX data ready check
 * \retval false The USART can not receive data to be transmitted
 * \retval true The USART can receive data to be transmitted
 */
bool USART0_IsTxReady();

/**
 * \brief Check if the USART has received data
 *
 * \return The status of USART RX data ready check
 * \retval true The USART has received data
 * \retval false The USART has not received data
 */
bool USART0_IsRxReady();

/**
 * \brief Check if USART0 data is transmitted
 *
 * \return Receiver ready status
 * \retval true  Data is not completely shifted out of the shift register
 * \retval false Data completely shifted out if the USART shift register
 */
bool USART0_IsTxBusy();



bool USART0_IsTxDone();
/**
 * \brief Read one character from USART0
 *
 * Function will block if a character is not available.
 *
 * \return Data read from the USART0 module
 */
uint8_t USART0_Read(void);

/**
 * \brief Write one character to USART0
 *
 * Function will block until a character can be accepted.
 *
 * \param[in] data The character to write to the USART
 *
 * \return Nothing
 */
void USART0_Write(const uint8_t data);

void USART0_flush();

/**
 * \brief Set call back function for USART0
 *
 * \param[in] cb The call back function to set
 *
 * \param[in] type The type of ISR to be set
 *
 * \return Nothing
 */
 
void USART0_DefaultRxIsrCb(void);

void USART0_DefaultTxIsrCb(void);

void USART0_SetISRCb(usart_callback cb, usart0_cb_t type);

void USART0_SetRXISRCb(usart_callback cb);

void USART0_SetTXISRCb(usart_callback cb);


#ifdef	__cplusplus
}
#endif

#endif	/* USART_H */


/*
 * UART.h
 *
 * Created: 050522
 *  Author: Njål-
 */ 

#ifndef UART_H_
#define UART_H_

#define F_CPU 16000000UL
// Includes
#include <avr/io.h>
#include <string.h>
#include <stdio.h>


// Størrelser
#define uart_bps 57600L  // max 57600
#define maxWordLength 16
#define txBufferlength 80
#define rxBufferLength 64

// Pins
#define PORTB_PB0_bp 0

// Registere og kontrollbits
#define UARTRxBuffer USART3_RXDATAL
#define UARTTxBuffer USART3_TXDATAL
#define UARTRxNotEmpty (USART3_STATUS & (1 << USART_RXCIF_bp))    // Filter out the Rx buffer empty bit from the UART control register
#define txBufferFree (USART3_STATUS & (1 << USART_DREIF_bp))    // Filter out the Tx buffer free bit from the UART control register

// Tastaturtaster
#define Bell 0x07
#define backspace 0x08
#define backspace127 127
#define CR 0x0D
#define LF 0x0A
#define CTRL_T 0x14


// Variabler
char txBuffer[txBufferlength];
extern volatile uint8_t rxBuffer[rxBufferLength];
extern volatile uint8_t ringbufferOut;       // The position for taking out data from the ring-buffer
extern volatile uint8_t ringbufferIn;        // The position for putting in received data
extern volatile uint8_t UARTEchoOn;
extern uint8_t noMoreBackSpace;
extern uint8_t ringBufferFull;

// Prototyper
void sendUARTecho(char);
void sendChar(char);
int sendCharPrintf(char, FILE *stream);
void sendBuffer(void);
void flushRxBuffer(void);
void sendNewLine(void);
void putCharInUARTBuffer(char);
void initUART(void);
char getBufferChar(void);

#endif /* UART_H_ */
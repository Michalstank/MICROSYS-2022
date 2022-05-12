/*
* UART.c
*
* Created: 050522
*  Author: Njål-
*/

#include "UART.h"

void sendUARTecho(char receivedByte){
	if (((receivedByte > 31) && (receivedByte != backspace127)) || (receivedByte == Bell) || (receivedByte == CR) || (receivedByte == LF)){
		if (ringBufferFull)  {
			UARTTxBuffer = Bell;
			} else{
			UARTTxBuffer = receivedByte;
		}
		noMoreBackSpace = 0;
	}
	
	if ((receivedByte == backspace) || (receivedByte == backspace127)) {
		if (ringbufferOut == ringbufferIn) {
			if (noMoreBackSpace == 0){
				noMoreBackSpace = 1;
				UARTTxBuffer = receivedByte;
				} else {
				UARTTxBuffer = Bell;
			}
			} else {
			UARTTxBuffer = receivedByte;
		}
	}
}

int sendCharPrintf(char charToSend, FILE *stream){
	if (charToSend == '\n') {
		sendCharPrintf('\r', stream);
	}
	while (!txBufferFree){
		// Loop here and do nothing while waiting for free TX buffer.
	}
	UARTTxBuffer = charToSend;
	return 0;
}

void sendChar(char charToSend){
	while (!txBufferFree){
		// Loop here and do nothing while waiting for free TX buffer.
	}
	UARTTxBuffer = charToSend;
}

void sendBuffer(void){
	for(uint8_t txBufferLoop=0; txBufferLoop < txBufferlength; txBufferLoop++){
		char tempTxChar;
		tempTxChar = txBuffer[txBufferLoop];
		if (tempTxChar!=0){
			sendChar(tempTxChar);
			}else{
			return;
		}
	}
}

void flushRxBuffer(void){
	ringbufferIn = 0;
	ringbufferOut = 0;
	ringBufferFull = 0;
}

void sendNewLine(void){
	sendChar(CR);
	sendChar(LF);
}

void putCharInUARTBuffer(char insertChar){
	if(insertChar == backspace || insertChar == backspace127){
		if (ringbufferIn != ringbufferOut){ // As long as the buffer is not empty, move back one character. NOT UTF-8 safe!
			if (ringbufferIn==0){
				ringbufferIn = rxBufferLength-1;
				}else{
				ringbufferIn--;
			}
			ringBufferFull = 0;
		}
		} else {

		// If the ring-buffer is claimed to be full, check if it is still true.
		if (ringBufferFull) {
			if( (ringbufferOut - ringbufferIn == 1) || ( (ringbufferIn == rxBufferLength-1) && (ringbufferOut == 0) ) ){
				// The buffer is still full, drop the byte.
				} else{
				//If the ring-buffer is not full anymore, clear the flag.
				ringBufferFull = 0;
			}
		}
		// If the ring-buffer is not full, put the char into the buffer.
		if (!ringBufferFull) {
			rxBuffer[ringbufferIn] = insertChar;

			// Check to see if the buffer now is full, if it is, set the flag.
			if( (ringbufferOut - ringbufferIn == 1) || ( (ringbufferIn == rxBufferLength-1) && (ringbufferOut == 0) ) ){
				ringBufferFull = 1;   // true
				} else {
				// Otherwise increase the buffer in-index
				ringbufferIn++;

				//Wrap the buffer if it has reached the end
				if (ringbufferIn >= rxBufferLength) {
					ringbufferIn = 0;
				}
			}
		}
	}
}


// char getBufferChar(void) {
// 	char returnChar;
// 	if (ringbufferIn == ringbufferOut){
// 		return 0;
// 	}
// 	else {
// 		returnChar = rxBuffer[ringbufferOut];
// 		ringbufferOut++;
// 		if (ringbufferOut >= rxBufferLength) {
// 			ringbufferOut = 0;
// 			return 0;
// 		}
// 		return returnChar;
// 	}
// }

void initUART (void){
	/* From datasheet
	Full Duplex Mode:
	1. Set the baud rate (USARTn.BAUD).
	2. Set the frame format and mode of operation (USARTn.CTRLC).
	3. Configure the TXD pin as an output.
	4. Enable the transmitter and the receiver (USARTn.CTRLB). */

	// calculating constant for baud rate register
	const uint16_t baudRateReg = (uint16_t)( 64 * F_CPU / ( 16 * uart_bps ));

	// Virtual serial port is connected to UART3
	
	// (1.) setting the baud rate register
	USART3_BAUD = baudRateReg;
	
	// (2.) Set the frame format and mode of operation (USARTn.CTRLC).
	USART3_CTRLC = 0x00 | (0 << USART_CMODE0_bp) | (0 << USART_PMODE0_bp) | (0 << USART_SBMODE_bp) | (3 << USART_CHSIZE0_bp);

	// (3.) connect UART3 to PB0 and PB1. UART2, UART1 and UART0 not connected to any pins
	PORTMUX_USARTROUTEA = 0x00 | (0 << PORTMUX_USART30_bp) | (3 << PORTMUX_USART20_bp) | (3 << PORTMUX_USART10_bp) | (3 << PORTMUX_USART00_bp);
	
	// (3.) set PB0 as output
	PORTB_DIRSET = (1 << PORTB_PB0_bp);

	// (4.) Enable the transmitter and the receiver (USARTn.CTRLB)
	USART3_CTRLA = 0x00 | (1 << USART_RXCIE_bp) | (0 << USART_TXCIE_bp) | (0 << USART_DREIE_bp) | (0 << USART_RXSIE_bp) | (0 << USART_LBME_bp) | (0 << USART_ABEIE_bp) | (0 << USART_RS485_bp) ;
	USART3_CTRLB = 0x00 | (1 << USART_RXEN_bp) | (1 << USART_TXEN_bp) | (0 << USART_SFDEN_bp) | (0 << USART_ODME_bp) | (1 << USART_RXMODE0_bp) | (0 << USART_MPCM_bp);

	USART3_CTRLD = 0x00 | (0 << USART_ABW0_bp);
}
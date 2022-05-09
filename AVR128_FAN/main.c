/*
* UART.h
*
* Created: 25.03.2022 14.35.57
*  Author: Njål-
*/

#define F_CPU 16000000UL

// Includes
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <util/delay.h>
#include <stdio.h>
#include "UART.h"
#include "menuGraphics.h"

char readNextProcessWord(void);

// Prototypes
void startupRoutine(void);
void initOsc(void);

// Global variables
volatile uint8_t rxBuffer[rxBufferLength];
volatile uint8_t ringbufferOut = 0;          // The position for taking out data from the ring-buffer
volatile uint8_t ringbufferIn = 0;           // The position for putting in received data
volatile uint8_t interpret = 0;
char processThisWord[maxWordLength + 1];
uint8_t noMoreBackSpace = 0;
uint8_t ringBufferFull = 0;

enum menus{mainMenu=0, fanMenu, settings, statistics};
enum menus currentMenu;

// Setter opp bruk av printf funksjon
static FILE stdoutFile = FDEV_SETUP_STREAM(sendCharPrintf, NULL, _FDEV_SETUP_WRITE);

int main (void) {
	startupRoutine();
	stdout = &stdoutFile;
	_delay_ms(1000);
	printf("___Welcome!____<");
	
	
	while (1) {
		// Main menyen
		if (interpret == 0) {
			_delay_ms(1000);
			printf("\n[Interpreting...]\n");
			printGraphics("mainMenu");
			currentMenu = mainMenu;
			interpret = 1;
			flushRxBuffer();
			printf("> ");
		}
		
		//Fan menyen
		if (strcmp(&rxBuffer,"1")==0 &&interpret == 1) {
			printf("\n[Interpreting...]\n");
			printGraphics("fanStateMenu");
			currentMenu = fanMenu;
			interpret = 2;
			flushRxBuffer();
			printf("> ");
		}
		
		//Settings menyen
		if (strcmp(&rxBuffer,"2")==0 && interpret == 1) {
			printf("\n[Interpreting...]\n");
			printGraphics("settingsMenu");
			currentMenu = settings;
			interpret = 2;
			flushRxBuffer();
			printf("> ");
		}
			// Overview menyen
		if (strcmp(&rxBuffer,"3")==0 && interpret == 1) {
			printf("\n[Interpreting...]\n");
			printGraphics("statisticsMenu");
			currentMenu = statistics;
			interpret = 2;
			flushRxBuffer();
			printf("> ");
		}
		
	}
	
// Koden herfra og ned er hentet fra eksempelkoden til Ingulf, med noen få forandringer
}
	ISR(USART3_RXC_vect){
		uint8_t receivedByte = UARTRxBuffer;
		static uint8_t lastReceivedByte = 0;
		switch (receivedByte){
			case (LF):
			if (lastReceivedByte == CR){         // If the last received byte was CR and the current one is LF then ignore the LF.
				break;
			}
			
			case (CR):
			putCharInUARTBuffer(receivedByte);
			putCharInUARTBuffer('\000');
			interpret = 0;	// Hvis <enter> blir mottatt starter tolking av input (interpret = 1).
			sendBuffer();
			break;
			
			default:
			putCharInUARTBuffer(receivedByte);
			break;
		}
		if (txBufferFree){
			//sendChar(receivedByte);
			sendUARTecho(receivedByte);
		}
		lastReceivedByte = receivedByte;
	}


	void startupRoutine() {
		initOsc();				// Overclocker
		initUART();				// initialize the UART
		sei();					// Global interrupt enable
		rxBuffer[0] = 0;		// Setting end of string character at the beginning of the Rx array;
	}

	void initOsc(void){
		// Overklokker prosessorhastigheten til 16MHz
		CCP = 0xD8;
		CLKCTRL_OSCHFCTRLA = 0x00 | (7 << CLKCTRL_FREQSEL0_bp );
	};
	//-------------------------------------------------------------------------------------------------------
	char readNextProcessWord(void) {
		for(uint8_t loopNo=0 ; loopNo < maxWordLength ; loopNo++){ // the for loop ensures max length
			uint8_t tempChar;
			tempChar = getBufferChar();
			if (tempChar == 0 && loopNo==0){
				return 0;
			}
			if (tempChar > 32) {
				processThisWord[loopNo]=tempChar;
				} else {
				processThisWord[loopNo]=0; // add null character to end string
				break; // and exit loop
			}
		}
		processThisWord[maxWordLength]=0; // make sure the end has a null-character
		return 1;
	}
	
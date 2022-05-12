/*
* main.c
*
* Created: 25.03.2022 14.35.57
*  Author: Njål-
*/

#define F_CPU 16000000UL
#define bufferDepth 24
#define maxWordLength 16
#define txBufferlength 80
#define rxBufferLength 64
// Includes
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <util/delay.h>
#include <stdio.h>
#include "UART.h"
#include "menuGraphics.h"
#include "TempRead.h"
#include "LCD_I2C.h"
#include "twi_master.h"
#include "menuCommands.h"

// Prototypes
void startupRoutine(void);
void initOsc(void);


// Global variables
volatile uint8_t rxBuffer[rxBufferLength];
volatile uint8_t ringbufferOut = 0;          // The position for taking out data from the ring-buffer
volatile uint8_t ringbufferIn = 0;           // The position for putting in received data
volatile uint8_t interpret= 0;
volatile uint8_t lcdState = 0;
volatile uint8_t controllState = 0;
volatile uint8_t rpmState = 0;
uint16_t lcdToggle = 0;
uint16_t controllToggle = 0;
char processThisWord[maxWordLength + 1]; // room for null-character
char txBuffer[txBufferlength];;
volatile uint8_t noMoreBackSpace = 0;
volatile uint8_t ringBufferFull = 0;


static int uart_putchar(char c, FILE *stream);



#define noOfCommands 4
#define maxCommandLength 5
const char commandArray[noOfCommands][maxCommandLength] = {"main","settings","static"," "};
enum menus{mainMenu=0, stateMenu, settings, statistics};
enum menus currentMenu;


// Setter opp bruk av printf funksjon
static FILE stdoutFile = FDEV_SETUP_STREAM(sendCharPrintf, NULL, _FDEV_SETUP_WRITE);



int main (void) {
	
	i2c_lcd_init();
	PORT_init();
	TCA1_init();
	_delay_ms(100);
	i2c_send_command(0x0D);
	startupRoutine();
	adc_init();
	stdout = &stdoutFile;
	_delay_ms(1000);
	printf("___Welcome!____<\n");
	_delay_ms(500);
	printGraphics("mainMenu");
	printf("> ");
	
	uint32_t temp = 0;
	uint16_t offset = SIGROW.TEMPSENSE1;
	uint16_t slope = SIGROW.TEMPSENSE0;
	float adc_reading = 0;
	float final_temp = 0;
	
	while (1) {
		
		if (interpret) {
			printf("\n[Interpreting...]\n");
			
			if (readNextProcessWord() != 0) {
				
		 if (strcmp(processThisWord,"main")==0) {
			 _delay_ms(150);
			 printGraphics("mainMenu");
			 //currentMenu = mainMenu;
			 flushRxBuffer();
			 interpret = 0;
			 _delay_ms(250);
			 printf("> %d", final_temp);
		 }

		 //Fan menyen
		 if (strcmp(processThisWord,"state")==0) {
			 printf("\n[Interpreting...]\n");
			 printGraphics("fanStateMenu");
			 currentMenu = stateMenu;
			 flushRxBuffer();
			 interpret = 0;
			 _delay_ms(250);
			 printf("> %d", interpret);
		 }



		 //Settings menyen
		 if (strcmp(processThisWord,"settings")==0) {
			 printf("\n[Interpreting...]\n");
			 printGraphics("settingsMenu");
			 currentMenu = settings;
			 flushRxBuffer();
			 interpret = 0;
			 _delay_ms(250);
			 printf("> ");
			 
		 }

		 if (strcmp(processThisWord,"LCD")==0) {
			 if(lcdToggle == 0){
				 printf("[Initializig the LCD]\n");
				 PORTC.DIRSET = PIN0_bm;
				 _delay_ms(150);
				 i2c_lcd_init();
				 _delay_ms(250);
				 i2c_lcd_send_string((unsigned char*)"  I2C is active  ");
				 i2c_set_cursor(2,1);
				 i2c_lcd_send_string((unsigned char*)"I2C adress: 0x27");
				 flushRxBuffer();
				 lcdToggle = 1;
				 lcdState = 1;
				 printGraphics("settingsMenu");
			 }
			 _delay_ms(250);
			 flushRxBuffer();
			 printf("> ");
		 }

		 if (strcmp(processThisWord,"toggle")==0) {
			 if(controllToggle == 0){
				 printf("[Toggling controll settings]\n");
				 
				 _delay_ms(1000);
				 flushRxBuffer();
				 controllToggle = 1;
				 controllState = 1;
				 printGraphics("settingsMenu");
			 }
			 _delay_ms(250);
			 flushRxBuffer();
			 printf("> ");
		 }
		 
		 if (strcmp(processThisWord,"manual")==0) {
			 printf("Please enter angle: ");	
			 _delay_ms(250);
			 flushRxBuffer();
			 interpret = 0;
		 }

		 // Overview menyen
		 if (strcmp(processThisWord,"static")==0) {
			 printf("[Interpreting...]\n");
			 printGraphics("statisticsMenu");
			 currentMenu = statistics;
			 interpret = 0;
			 _delay_ms(250);
			 flushRxBuffer();
			 printf("> ");
		 }
		 else{
			 _delay_ms(500);
			 printf("[Unknown command!]\n");
			 printf("> ");
		 interpret = 0;
		 }

		 }
		 
		}
	}
}

// Koden herfra og ned er hentet fra eksempelkoden til Ingulf, med noen få forandringer


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
		interpret = 1;	// Hvis <enter> blir mottatt starter tolking av input (interpret = 1).
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
	rxBuffer[0] = 0;		// Setting end of string character at the beginning of the Rx array
}

void initOsc(void){
	// Overklokker prosessorhastigheten til 16MHz
	CCP = 0xD8;
	CLKCTRL_OSCHFCTRLA = 0x00 | (7 << CLKCTRL_FREQSEL0_bp );
};

//-------------------------------------------------------------------------------------------------------

char getBufferChar(void) {
	char returnChar;
	if (ringbufferIn == ringbufferOut){
		return 0;
		} else {
		returnChar = rxBuffer[ringbufferOut];
		ringbufferOut++;
		if (ringbufferOut >= bufferDepth) {
			ringbufferOut = 0;
		}
		return returnChar;
	}
}


void readNextProcessWord(void) {
	for(uint16_t loopNo=0 ; loopNo < maxWordLength ; loopNo++){ // the for loop ensures max length
		uint16_t tempChar;
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

char findCommand(void){
	for (uint8_t compareAgainstCommand=0 ; compareAgainstCommand < noOfCommands ; compareAgainstCommand++){
		for (uint8_t compareCharacterNumber = 0 ; compareCharacterNumber < maxCommandLength ; compareCharacterNumber++){
			if (commandArray[compareAgainstCommand][compareCharacterNumber] == processThisWord[compareCharacterNumber]) {
				if (processThisWord[compareCharacterNumber] == 0) {
					return compareAgainstCommand + 1;
				}
				} else {
				compareCharacterNumber = maxCommandLength; // jump to the next command
			}
		}
	}
	return 0;
}

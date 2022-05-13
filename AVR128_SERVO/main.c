/*
* main.c
*
* Created: 25.03.2022 14.35.57
*  Author: Njål-
*/

// Generelt om koden;
// De fleste driverene fungerer bra. PWM driveren har noen merkelige verdier, som dutycycle mellom 15000 og 42500. 
// Har probet utgangene med osiloskop, fant ut at ved en periodetid på 0xFFF er det 246Hz i frekvens. PWM driveren
// Ville ha frekvensen i MHz for det meste, så dette ble det laveste som jeg kom frem til

// Det er er merke nede i Koden, denne inikerer starten på UART/PuttY printingen. De fleste av disse skulle vært i 
// MenuCommands.C, men jeg har ikke fått tid til å flytte de over. Er mulig, men må gjøre noe smart med alle variablene de 
// beytter seg av når de skriver til Putty.

// Her er en del _delay_ms og extern volitile brukt i koden. Burde se på muligheter for å unngå disse. Kan legge inn en form for millis()
// Men dette vil ta tid vi ikke lenger har til dette prosjektet. 

// Akuratt nå har ikke koden noen form for Ekstern styring, dette burde sees på, om det er tid kan jeg få til en TWI_slave driver, men
// dette tar ikke prioritet akkurat nå

// Mye av UART greie er likt det Ingulf har gitt oss under emnet. Har sett på måter å lage noe nytt, men dette tar mye tid, og i mange
// tilfeller gjør det akkurat det samme som koden til Ingulf

// En del av koden har fått inspirasjon fra nettet. Dette vil bli henvist til i rapporten

// Kommadoer i Putty
// main - hovedmeny
// settings - instillinger
// static - oversikt over status
// state - viser litt mer spesifikt for servoen
// menyene nemnt over kan egentlig slåes sammen
// toggle - skrur av eller på manuell styring av servo
// LCD - skrur på LCD
// degrees - gjør det mulig å skrive inn grader, må låses opp en gang
// degrees er en teit funksjon, men var tenkt å brukes med flere automatiske funksjonaliteter, men kom aldri så langt


// Ting som bør gjøres:
// Akkurat nå har vi ingen måte å køre automatisk på
// - henter inn temperatur fra AVR'en. kan bruke dette
// LCD skjermen viser svært lite
// Inge styring fra eksterne kilder
// menuCommands bør fylles ut, og main bør bli mindre
// Ingen måling av strøm, kan også bruke dette for å kompansere for ting i servoen
// Det er ferdigsatte grader servoen kan stilles til, man kan ikke velge fritt, burde gjøre noe
// Det er ikke mulig å justere hastigheten til servoen

//Ting som må Fikses:
//twi_write_adress() funksjon | Infinite Loop
//
//
//
//


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
#include "PWM.h"


// Prototypes
void startupRoutine(void);
void initOsc(void);


// Global variables
volatile uint8_t rxBuffer[rxBufferLength];
volatile uint8_t ringbufferOut = 0;          // The position for taking out data from the ring-buffer
volatile uint8_t ringbufferIn = 0;           // The position for putting in received data
volatile uint8_t interpret= 0;
volatile uint8_t lcdState = 0;
volatile uint8_t degrees = 0;
volatile uint8_t controllState = 0;
volatile uint8_t rpmState = 0;

// Variabler brukt for å fortelle noen om tilstanden til servo/ kontroller
uint16_t LCD_toggle = 0;
uint8_t angle_lock = 1;
uint16_t controll_toggle = 0;
uint16_t auto_degrees = 0;

// VAriabler for UART
char processThisWord[maxWordLength + 1]; // room for null-character
char txBuffer[txBufferlength];;
volatile uint8_t noMoreBackSpace = 0;
volatile uint8_t ringBufferFull = 0;
float final_temp = 0;

static int uart_putchar(char c, FILE *stream);
#define noOfCommands 4
#define maxCommandLength 5
// Dette blir ikke brukt, men hadde vært en ide å benytte en switch/case
const char commandArray[noOfCommands][maxCommandLength] = {"mainMenu","settings","static"," "};
enum menus{mainMenu=0, stateMenu, settings, statistics};
enum menus currentMenu;


// Setter opp printf funksjon
static FILE stdoutFile = FDEV_SETUP_STREAM(sendCharPrintf, NULL, _FDEV_SETUP_WRITE);


int main (void) {
	
// Initialiserer alt som trenger det

	//i2c_lcd_init();
	PORT_init();
	TCA1_init();
	_delay_ms(100);
	i2c_send_command(0x0D);
	startupRoutine();
	adc_init();
	stdout = &stdoutFile;
	_delay_ms(1000);
	// Koselig startup
	printf("___Welcome!____<\n");
	_delay_ms(500);
	menu_printer("mainMenu");
	printf("> ");
	
// Noen verdier for temperarutemålingen. Kan egentlig fyttes ut fra main
	uint32_t temp = 0;
	uint16_t offset = SIGROW.TEMPSENSE1;
	uint16_t slope = SIGROW.TEMPSENSE0;
	float adc_reading = 0;
	float final_temp = 0;
	
// Hoved-loopen
	while (1) {
		
		for(int i = 0x0000; i < 0x9C40; i+=0x0010){
			TCA1.SINGLE.CMP2 = i;
			_delay_ms(10);
		}
		
		// BEgynner å sjekke RxBufferet om interpret == 1
		if (interpret) {
			printf("\n[Interpreting...]\n");
			
			if (readNextProcessWord() != 0) { //Merket som er nevnt i starten av koden ;)
				
				// Hovedmenyen
				if (strcmp(processThisWord,"main")==0) {
					_delay_ms(150);
					menu_printer("mainMenu");
					currentMenu = mainMenu;
					flushRxBuffer();
					interpret = 0;
					_delay_ms(250);		
				}

				//Servomenyen menyen, lider fortsatt av Fan menyen (trudde dette skulle bli oppgave 2)
				if (strcmp(processThisWord,"state")==0) {
					printf("\n[Interpreting...]\n");
					menu_printer("fanStateMenu");
					printf("%d",auto_degrees);
					currentMenu = stateMenu;
					flushRxBuffer();
					interpret = 0;
					_delay_ms(250);
				}

				//Settings menyen
				if (strcmp(processThisWord,"settings")==0) {
					printf("\n[Interpreting...]\n");
					menu_printer("settingsMenu");
					currentMenu = settings;
					flushRxBuffer();
					interpret = 0;
					_delay_ms(250);	
				}
				
				// Skrur på LCD-skjermen. Denne kan toggles av og på
				if (strcmp(processThisWord,"LCD")==0) {
					if(LCD_toggle == 0){
						printf("[Initializig the LCD]\n");
						PORTC.DIRSET = PIN0_bm;
						_delay_ms(150);
						i2c_lcd_init();
						_delay_ms(250);
						i2c_lcd_send_string((unsigned char*)"  I2C is active  ");
						i2c_set_cursor(2,1);
						i2c_lcd_send_string((unsigned char*)"I2C adress: 0x27");
						flushRxBuffer();
						_delay_ms(1000);
						LCD_toggle = 1;
						lcdState = 1;
					}
					else{
						PORTC.OUTSET = PIN0_bm;
						_delay_ms(150);
						LCD_toggle = 0;
						lcdState = 0;
					}
					interpret = 0;
					menu_printer("settingsMenu");
					_delay_ms(250);
					flushRxBuffer();
					printf("> ");
				}

				// Toggler styring av servo mellom manuell og automagisk
				if (strcmp(processThisWord,"toggle")==0) {
					printf("[Toggling controll settings]\n");
					_delay_ms(500);
					flushRxBuffer();
					if (controll_toggle == 0 ){
						controll_toggle = 1;
						controllState = 1;
						angle_lock = 0;
					}
					else{
						controll_toggle = 0;
						controllState = 0;
						angle_lock = 1;
					}
					
					menu_printer("settingsMenu");
					_delay_ms(250);
					flushRxBuffer();
					printf("> ");
				}
				
				// Låser opp muligheten for å legge inn forhåndsdefinerte vinkler
				if (strcmp(processThisWord,"degrees")==0 && controll_toggle == 1) {
					printf("Please enter angle: ");
					angle_lock = 0;
					PORT_init();
					TCA1_init();
					_delay_ms(250);
					flushRxBuffer();
					interpret = 0;
				}
				
				// Herfra og et stykke ned er det kun styring av servoen, veldig brute force
				if (strcmp(processThisWord,"0")==0 && angle_lock == 0){
					degrees = 0;
					controll_toggle = 0;
					menu_printer("settingsMenu");
					TCA1.SINGLE.CMP2 = 15000;
					interpret = 0;
					flushRxBuffer();
					printf("> ");
				}
				if (strcmp(processThisWord,"45")==0 && angle_lock == 0){
					degrees = 45;
					controll_toggle = 0;
					menu_printer("settingsMenu");
					TCA1.SINGLE.CMP2 = 22000;
					interpret = 0;
					flushRxBuffer();
					printf("> ");
				}
				if (strcmp(processThisWord,"90")==0 && angle_lock == 0){
					degrees = 90;
					controll_toggle = 0;
					menu_printer("settingsMenu");
					TCA1.SINGLE.CMP2 = 31000;
					interpret = 0;
					flushRxBuffer();
					printf("> ");
				}
				if (strcmp(processThisWord,"135")==0 && angle_lock == 0){
					degrees = 135;
					controll_toggle = 0;
					menu_printer("settingsMenu");
					TCA1.SINGLE.CMP2 = 39000;
					flushRxBuffer();
					interpret = 0;
					flushRxBuffer();
					printf("> ");
				}
				if (strcmp(processThisWord,"180")==0 && angle_lock == 0){
					degrees = 180;
					controll_toggle = 0;
					menu_printer("settingsMenu");
					TCA1.SINGLE.CMP2 = 42500;
					flushRxBuffer();
					interpret = 0;
					flushRxBuffer();
					printf("> ");
				}
				// Denne funksjonen funker ikke. Tanken er at den skal hente inn den interne temperaturen til kontrolleren
				// for så å gjøre dette om til grader. Har ikke fått tiden til å se på det enda, er litt rot med forandringer
				// mellom float og int i temp-måle funksjonen
				// Dersom strømtrekk funker kan denne også bygges ut til å kompansere for treghet i spjeld
				if (angle_lock == 1){
					PORT_init();
					TCA1_init();
					auto_degrees =  (int)final_temp;
					//auto_degrees = auto_degrees/30;
					TCA1.SINGLE.CMP2 = auto_degrees;	
				}
				
				
				// Overview menyen
				if (strcmp(processThisWord,"static")==0) {
					
					adc_init();
					uint32_t temp = 0;
					uint16_t offset = SIGROW.TEMPSENSE1;
					uint16_t slope = SIGROW.TEMPSENSE0;
					float adc_reading = 0;
					volatile char screen_temp[1];
					printf("[Interpreting...]\n");
					start_conversion();
					_delay_ms(150);
					adc_reading = ADC0.RES;
					adc_reading /= 16.0f;
					temp = offset - adc_reading;
					temp *= slope;
					temp += 256;//to get a 1/16 kelvin precision
					temp >>= 9;
					final_temp = temp;
					final_temp *= 0.125f;
					final_temp -= 273.15f;// convert to Degrees
					
					dtostrf(final_temp,4,1, screen_temp);
					
					printf("-------[OVERVIEW]-------\n");
					printf("[Current angle:    %d ", degrees);
					printf("]");
					printf("\n[Current Temp:     %s", screen_temp);
					printf("]");
					printf("\n[Mode:          ");
					if(angle_lock == 1){
						printf("   AUTO]\n");
					}
					else{
						printf(" Locked]\n");
					}

					menu_printer("statisticsMenu");

					currentMenu = statistics;
					interpret = 0;
					_delay_ms(250);
					flushRxBuffer();
					printf("> ");
				}
				// Ukjente kommandoer får også svar
				if(interpret){
					_delay_ms(150);
					printf("[Unknown command!]\n");
					printf("> ");
					interpret = 0;
				}

			}
		}		
	}
}



// Koden herfra og ned er hentet fra eksempelkoden til Ingulf, med noen få forandringer
// Burde se på muligheter for å få lagt den inn i UART.C filen. Prøvde på det, men slet med å kalle på funksjonene under
// Hadde sikkert gått med mer tid

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

void findCommand(void){
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

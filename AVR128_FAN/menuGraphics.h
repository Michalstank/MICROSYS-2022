/*
* UART.c
*
* Created: 050522
*  Author: Njål-
*/

#ifndef menuGraphics_H_
#define menuGraphics_H_

#include <avr/io.h>
#include <string.h>
#include <util/delay.h>

// Terminal definisjonar
#define maxMenuLineLenght 80
#define terminalHeight 24

// Makroar for unike rader
#define rowAlertStart 0
#define rowMainMenuOptionStart 13
#define rowRPMValuesStart 7
#define rowSettingMenuOptionStart 9
#define rowStatisticsMenuStart 7

// Andre Makroar
#define numberOfFans 8
#define maxToggleSettingLenght 4
#define numberOfToggleSettings 2

// Makroar for menyfargar
//#define defaultColor "\x1b[37;44m"
//#define alertType1Color "\x1b[37;41m"
//#define alertType2Color "\x1b[37;46m"

#define numberOfAlerts 3
#define maxAlertLenght 8

// Generelle makroar
#define decimal 10
#define TRUE 1
#define FALSE 0

// Prototypar
void initScreen(void);
void printGraphics(char *menuSelect);
uint8_t checkForAlerts(uint8_t);

// Structs
typedef struct {
	uint8_t state;
	char stateName[4];
} onOffButton;

#endif
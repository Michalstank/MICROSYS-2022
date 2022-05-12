// #include "menuCommands.h"
// 
// void menuState(void){
// 
// if (strcmp(&rxBuffer,"main")==0) {
// 	_delay_ms(1000);
// 	printf("\n[Interpreting...]\n");
// 	printGraphics("mainMenu");
// 	currentMenu = mainMenu;
// 	flushRxBuffer();
// 	interpret = 0;
// 	_delay_ms(250);
// 	printf(">");
// }
// 
// //Fan menyen
// if (strcmp(&rxBuffer,"1")==0) {
// 	printf("\n[Interpreting...]\n");
// 	printGraphics("fanStateMenu");
// 	currentMenu = stateMenu;
// 	flushRxBuffer();
// 	interpret = 0;
// 	_delay_ms(250);
// 	printf("> %d", interpret);
// }
// 
// 
// 
// //Settings menyen
// if (strcmp(&rxBuffer,"2")==0) {
// 	printf("\n[Interpreting...]\n");
// 	printGraphics("settingsMenu");
// 	currentMenu = settings;
// 	flushRxBuffer();
// 	interpret = 0;
// 	_delay_ms(500);
// 	printf("> %d", interpret);
// 	
// }
// 
// if (strcmp(&rxBuffer,"3")==0) {
// 	if(lcdToggle == 0){
// 		printf("[Initializig the LCD]\n");
// 		PORTC.DIRSET = PIN0_bm;
// 		_delay_ms(150);
// 		i2c_lcd_init();
// 		_delay_ms(250);
// 		i2c_lcd_send_string((unsigned char*)"  I2C is active  ");
// 		i2c_set_cursor(2,1);
// 		i2c_lcd_send_string((unsigned char*)"I2C adress: 0x27");
// 		flushRxBuffer();
// 		lcdToggle |= 1;
// 		lcdState = 1;
// 		printGraphics("settingsMenu");
// 	}
// 	_delay_ms(250);
// 	flushRxBuffer();
// 	printf("> ");
// }
// 
// if (strcmp(&rxBuffer,"4")==0) {
// 	if(controllToggle == 0){
// 		printf("[Toggling controll settings]\n");
// 		_delay_ms(1000);
// 		flushRxBuffer();
// 		controllToggle = 1;
// 		controllState = 1;
// 		printGraphics("settingsMenu");
// 	}
// 	_delay_ms(250);
// 	flushRxBuffer();
// 	printf("> ");
// }
// 
// // Overview menyen
// if (strcmp(&rxBuffer,"5")==0) {
// 	printf("[Interpreting...]\n");
// 	printGraphics("statisticsMenu");
// 	currentMenu = statistics;
// 	interpret = 0;
// 	_delay_ms(250);
// 	flushRxBuffer();
// 	printf("> ");
// }
// }

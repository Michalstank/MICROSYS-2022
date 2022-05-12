// #include "menuCommands.h"
// 
// void menuState(void){
// 
// if (strcmp(&rxBuffer,"main")==0) {
// 	_delay_ms(1000);
// 	printf("\n[Interpreting...]\n");
// 	menu_printer("mainMenu");
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
// 	menu_printer("fanStateMenu");
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
// 	menu_printer("settingsMenu");
// 	currentMenu = settings;
// 	flushRxBuffer();
// 	interpret = 0;
// 	_delay_ms(500);
// 	printf("> %d", interpret);
// 	
// }
// 
// if (strcmp(&rxBuffer,"3")==0) {
// 	if(LCD_toggle == 0){
// 		printf("[Initializig the LCD]\n");
// 		PORTC.DIRSET = PIN0_bm;
// 		_delay_ms(150);
// 		i2c_lcd_init();
// 		_delay_ms(250);
// 		i2c_lcd_send_string((unsigned char*)"  I2C is active  ");
// 		i2c_set_cursor(2,1);
// 		i2c_lcd_send_string((unsigned char*)"I2C adress: 0x27");
// 		flushRxBuffer();
// 		LCD_toggle |= 1;
// 		lcdState = 1;
// 		menu_printer("settingsMenu");
// 	}
// 	_delay_ms(250);
// 	flushRxBuffer();
// 	printf("> ");
// }
// 
// if (strcmp(&rxBuffer,"4")==0) {
// 	if(controll_toggle == 0){
// 		printf("[Toggling controll settings]\n");
// 		_delay_ms(1000);
// 		flushRxBuffer();
// 		controll_toggle = 1;
// 		controllState = 1;
// 		menu_printer("settingsMenu");
// 	}
// 	_delay_ms(250);
// 	flushRxBuffer();
// 	printf("> ");
// }
// 
// // Overview menyen
// if (strcmp(&rxBuffer,"5")==0) {
// 	printf("[Interpreting...]\n");
// 	menu_printer("statisticsMenu");
// 	currentMenu = statistics;
// 	interpret = 0;
// 	_delay_ms(250);
// 	flushRxBuffer();
// 	printf("> ");
// }
// }

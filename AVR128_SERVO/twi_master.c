#include "twi_master.h"
#include "LCD_I2C.h"

void TWI_init(void){
	
	//PA2: SDA   PA3: SCL
	//Initialisering av TWI
	// SDA har 8 clockesykluser som setup tid
	TWI0.CTRLA |= 1 << 4;
	// SDA har 50ns med hold time
	TWI0.CTRLA |= 1 << 2;
	// TWI kjører i standard modus
	// Twinger TWI bussen i pause for uthenting av informasjo
	TWI0.MSTATUS = 0x01;
	// Tømmer flagbit
	TWI0.MSTATUS |= 1 << 6 | 1 << 5;
	TWI0.MBAUD = 14;
	// Skrur på TWI
	TWI0.MCTRLA |= 1 << 0;
}

// bool twi_write_adress(uint8_t adress)
// {
// 	// Adresse og write
// 	TWI0.MADDR = adress << 1;
// 	// Setter opp en eier for TWI bussen (master)
// 	while((TWI0.MSTATUS & 0x02) != 0x02);
// 	
// 	while(!WRITE_FLAG);
// 	
// 	if (RECEIVE_ACK){
// 		
// 		send_stop();
// 		return false;
// 	}
// 	
// 	return true;
// }

void send_stop(void){
	
	TWI0.MCTRLB = 0x03;
}

// Sender data på TWI bussen
void twi_send_data(unsigned char data){
	
	TWI0.MDATA = data;
	while(!(WRITE_FLAG && !RECEIVE_ACK));
}
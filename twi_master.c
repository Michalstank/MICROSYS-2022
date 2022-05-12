#include "twi_master.h"
#include "LCD_I2C.h"

void twi_init(void)
{
	//PA2: SDA   PA3: SCL
	TWI0.CTRLA |= 1 << 4;//SDA setup time is 8 clock
	TWI0.CTRLA |= 1 << 2;//50ns SDA hold time
	//Operating in standard mode
	TWI0.MSTATUS = 0x01;//force bus to idle
	TWI0.MSTATUS |= 1 << 6 | 1 << 5;//Clear Flags
	TWI0.MBAUD = 14;
	
	TWI0.MCTRLA |= 1 << 0;//Enable TWI
}

bool twi_write_adress(uint8_t adress)
{
	TWI0.MADDR = adress << 1;//adress+write
	while((TWI0.MSTATUS & 0x02) != 0x02);//Bus owner
	
	while(!WRITE_FLAG);
	
	if (RECEIVE_ACK)
	{
		send_stop();//send stop
		return false;
	}
	
	return true;
}

void send_stop(void)
{
	TWI0.MCTRLB = 0x03;
}

void twi_send_data(unsigned char data)
{
	TWI0.MDATA = data;
	while(!(WRITE_FLAG && !RECEIVE_ACK));
}
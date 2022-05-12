#include "LCD_I2C.h"

char lcd_buffer[1];

void i2c_lcd_init(void)
{
	_delay_ms(40);
	
	twi_init();
	if (twi_write_adress(ADRESS))
	{
		i2c_lcd_cmd(0x20);
		i2c_lcd_cmd(0x28);
		i2c_lcd_cmd(0x0C);
		i2c_lcd_cmd(0x06);
		i2c_lcd_cmd(0x01);
		send_stop();
	}
}

void i2c_lcd_cmd(unsigned char cmd)
{
	twi_send_data((cmd & 0xF0) | 0x0C);
	twi_send_data((cmd & 0xF0) | 0x08);
	
	twi_send_data((cmd << 4) | 0x0C);
	twi_send_data((cmd << 4) | 0x08);
	_delay_ms(2);
}

void i2c_lcd_data(unsigned char cmd)
{
	twi_send_data((cmd & 0xF0) | 0x0D);
	twi_send_data((cmd & 0xF0) | 0x09);
	
	twi_send_data((cmd << 4) | 0x0D);
	twi_send_data((cmd << 4) | 0x09);
	_delay_ms(2);
}

void i2c_lcd_send_string(unsigned char str[])
{
	if (twi_write_adress(ADRESS))
	{
		while(*str)
		{
			i2c_lcd_data(*str++);
		}
		send_stop();
	}
}

void i2c_set_cursor(uint8_t line, uint8_t columns){
	
	if (twi_write_adress(ADRESS))
	{
		if (line == 2)
		{
			i2c_lcd_cmd(0xBF+columns);
		}
		else
		{
			i2c_lcd_cmd(0x7F+columns);
		}
		
		send_stop();
	}
	
}
void i2c_print_float(float f){
	
	dtostrf(f,4,1, lcd_buffer);
	
	i2c_lcd_send_string((unsigned char*) lcd_buffer);
}



void i2c_send_command(uint8_t command){
	
	if (twi_write_adress(ADRESS))
	{
		i2c_lcd_cmd(command);
		send_stop();
	}

}
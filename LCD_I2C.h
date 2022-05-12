#ifndef LCD_I2C_H_
#define LCD_I2C_H_

#include "twi_master.h"
#include <util/delay.h>

void i2c_lcd_init(void);
void i2c_lcd_cmd(unsigned char);
void i2c_lcd_data(unsigned char);
void i2c_lcd_send_string(unsigned char str[]);
void i2c_set_cursor(uint8_t, uint8_t);
void i2c_print_float(float);
void i2c_send_command(uint8_t command);


#endif
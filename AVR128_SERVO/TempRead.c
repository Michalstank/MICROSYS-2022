#include "TempRead.h"
#include "LCD_I2C.h"

void adc_init(void)

{
	VREF.ADC0REF = 0x01;//Internal 2.048V reference
	_delay_us(60);//wait start up time
	ADC0.CTRLB = 0x04;//do 16 conversion
	ADC0.CTRLC = 0x02;//Clock divided by 8
	ADC0.MUXPOS = 0x42;//Temperature sensor channel
	ADC0.CTRLA |= 1 << 0;//Enable ADC0

}

void start_conversion(void){
	ADC0.COMMAND |= 1 << 0;//start conversion
	while(!(ADC0.INTFLAGS & 1 << 0));//wait until conversion is finish
}


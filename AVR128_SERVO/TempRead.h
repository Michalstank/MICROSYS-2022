#ifndef TempRead_H_
#define TempRead_H_

#include <avr/io.h>
#include <string.h>
#include <util/delay.h>

// Generelle makroar
#define decimal 10
#define TRUE 1
#define FALSE 0

void adc_init(void);

void start_conversion(void);


#endif
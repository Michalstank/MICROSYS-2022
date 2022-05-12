#ifndef PWM_H_
#define PWM_H_

#include <avr/io.h>
#include <string.h>
#include <stdio.h>

uint32_t dutyCycle = 0;
#define PERIOD_EXAMPLE_VALUE (0xFFFFF)

void PORT_init(void);
void TCA1_init(void);

#endif /* PWM_H_ */
#ifndef PWM_H_
#define PWM_H_

#include <avr/io.h>
#include <string.h>
#include <stdio.h>

uint32_t dutyCycle = 0;
uint32_t rate = 0x0001;

void PORT_init(void);
void TCA1_init(void);
void PWM_controlInit(void);

void PWM_rateUpdate(void);
void PWM_dutyCycleUpdate(bool val);
void PWM_update(uint32_t val);


#endif /* PWM_H_ */
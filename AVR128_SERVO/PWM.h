#ifndef PWM_H_
#define PWM_H_

#include <avr/io.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

int dutyCycle;
int rate;

void PORT_init(void);
void TCA1_init(void);
void PWM_buttonSetUp(void);

void PWM_rateUpdate(void);
void PWM_dutyCycleUpdate(bool val);
void PWM_update(int val);

#endif /* PWM_H_ */
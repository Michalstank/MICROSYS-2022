#include "PWM.h"

dutyCycle = 0x186A;
rate = 0x0010;

void TCA1_init(void)
{
	PORTMUX.TCAROUTEA = PORTMUX_TCA10_bm;				/* set waveform output on PORT C */

	TCA1.SINGLE.CTRLB = TCA_SINGLE_CMP2EN_bm | TCA_SINGLE_WGMODE_SINGLESLOPE_gc ; /*PWM - Single slope*/
	
	TCA1.SINGLE.PER = 0x9C40;							// 50hz frequency for pwm to match servo assuming we have 16Mh as clock speed
	TCA1.SINGLE.CMP2 = 0x186A;							/* set PWM to Medium*/
	
	TCA1.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV4_gc		/* divide Processor Clock by 4 so we can get 50hz within 4bits */
	| TCA_SINGLE_ENABLE_bm;								/* start timer */

}

void PORT_init(void)
{
	PORTC.DIRSET |= PIN6_bm;							/* set pin 6 of PORT C as output */
}

void PWM_buttonSetUp(void){
	
	//define 3 inputs, 1/2 for servo Direction, 3 for rate change
	
	//PIN SETUP: DISABLE PULLUP RESISITOR, INTERRUPT, INTERNAL BUFFER
	PORTE.DIRCLR = PIN1_bm;
	PORTE.DIRCLR = PIN2_bm;
	PORTE.DIRCLR = PIN3_bm;
	
// 	PORTE.PIN1CTRL |= PORT_PULLUPEN_bm;
// 	PORTE.PIN2CTRL |= PORT_PULLUPEN_bm;
// 	PORTE.PIN3CTRL |= PORT_PULLUPEN_bm;
// 	
}

void PWM_rateUpdate(void){
	//change hastighet vi increaser/decreaser dutycycle på.
	if(rate < 0x0100){
		rate *= 2;
	} else {
		rate =0x0001;
	}
}

void PWM_dutyCycleUpdate(bool val){
	//function called when either button 1/2 is pressed to increase/decrease dutycycle;
	if(val){
		//rotates servo to left
		dutyCycle -= rate;
		//if dutyCycle falls under 0 revert it back to 0;
		if(dutyCycle < 0x07D0){
			dutyCycle = 0x07D0;
		}
	} else {
		//rotates servo to right
		dutyCycle += rate;
		//if DutyCycle becomes larger than max value revert it back to max value 
		if(dutyCycle > 0x2904){
			dutyCycle = 0x2904;
		}
	}
}

void PWM_update(int val){
	if(val <= 0x2904 && val >= 0x07D0){
		TCA1.SINGLE.CMP2 = val;	//set new Value for Duty-cycle
	}				
}
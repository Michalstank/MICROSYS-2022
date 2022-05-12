#include "PWM.h"

void TCA1_init(void)
{
	PORTMUX.TCAROUTEA = PORTMUX_TCA10_bm;				/* set waveform output on PORT C */

	TCA1.SINGLE.CTRLB = TCA_SINGLE_CMP2EN_bm | TCA_SINGLE_WGMODE_SINGLESLOPE_gc ; /*PWM - Single slope*/
	
	TCA1.SINGLE.PER = 0x9C40							// 50hz frequency for pwm to match servo assuming we have 16Mh as clock speed
	TCA1.SINGLE.CMP2 = 0x0000;							/* set PWM to low*/
	
	TCA1.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV4_gc		/* divide Processor Clock by 4 so we can get 50hz within 4bits */
	| TCA_SINGLE_ENABLE_bm;								/* start timer */

}

void PORT_init(void)
{
	PORTC.DIRSET |= PIN6_bm;							/* set pin 6 of PORT C as output */
}

void PWM_controlInit(){
	
	//define 3 inputs, 1/2 for servo Direction, 3 for rate change
	
	PORTB.OUT = 0x00; //disable output
	
	PORTB.DIRSET = 0x00; //set port as input
}

void PWM_rateUpdate(){
	//change hastighet vi increaser/decreaser dutycycle på.
	if(rate < 0x0040){
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
		if(dutyCycle < 0x0000){
			dutyCycle = 0x0000;
		}
	} else {
		//rotates servo to right
		dutyCycle += rate;
		//if DutyCycle becomes larger than max value revert it back to max value 
		if(dutyCycle > 0x9C40){
			dutyCycle = 0x9C40;
		}
	}
}

void PWM_update(uint32_t val){
	if(val <= 0x9C40 && val >= 0x0000){
		TCA1.SINGLE.CMP2 = val;	//set new Value for Duty-cycle
	}				
}
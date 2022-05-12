#include "PWM.h"

void TCA1_init(void){
	
	//Bestemmer bølgeformen
	PORTMUX.TCAROUTEA = PORTMUX_TCA10_bm;			

	//SKru på comperator 2
	TCA1.SINGLE.CTRLB = TCA_SINGLE_CMP2EN_bm
	
	// Single slope PWM
	| TCA_SINGLE_WGMODE_SINGLESLOPE_gc ;
	
	// PWM frekvens, kan leke litt med denne, servoen vi bruker hadde 50Hz som frekvens
	TCA1.SINGLE.PER = PERIOD_EXAMPLE_VALUE;
	
	//PWM signalet trenger en intern klokke, setter klokkekilde
	TCA1.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV1_gc
	//Starter timer
	| TCA_SINGLE_ENABLE_bm;

}
void PORT_init(void){ 
	
	// Pinne 6 på port C er utgang for PWM
	PORTC.DIRSET |= PIN6_bm;
}
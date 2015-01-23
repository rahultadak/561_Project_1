#include "timers.h"
#include <MKL25Z4_accel.h>
#include "project1.h"
#include "board.h"
#include "app_init.h"

volatile unsigned PIT_interrupt_counter = 0;
volatile unsigned LCD_update_requested = 0;

void Init_PIT(unsigned period0,unsigned period1) {
	// Enable clock to PIT module
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;
	
	// Enable module, freeze timers in debug mode
	PIT->MCR &= ~PIT_MCR_MDIS_MASK;
	PIT->MCR |= PIT_MCR_FRZ_MASK;
	
	// Initialize PIT0 to count down from argument 
	PIT->CHANNEL[0].LDVAL = PIT_LDVAL_TSV(period0);

	// No chaining
	PIT->CHANNEL[0].TCTRL &= PIT_TCTRL_CHN_MASK;
	
	// Generate interrupts
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TIE_MASK;

		// Initialize PIT1 to count down from argument 
	PIT->CHANNEL[1].LDVAL = PIT_LDVAL_TSV(period1);

	// No chaining
	PIT->CHANNEL[1].TCTRL &= PIT_TCTRL_CHN_MASK;
	
	// Generate interrupts
	PIT->CHANNEL[1].TCTRL |= PIT_TCTRL_TIE_MASK;
	
	/* Enable Interrupts */
	NVIC_SetPriority(PIT_IRQn, 128); // 0, 64, 128 or 192
	NVIC_ClearPendingIRQ(PIT_IRQn); 
	NVIC_EnableIRQ(PIT_IRQn);	
}


void Start_PIT0(void) {
// Enable counter
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK;
}

void Stop_PIT0(void) {
// Enable counter
	PIT->CHANNEL[0].TCTRL &= ~PIT_TCTRL_TEN_MASK;
}

void Start_PIT1(void) {
// Enable counter
	PIT->CHANNEL[1].TCTRL |= PIT_TCTRL_TEN_MASK;
}

void Stop_PIT1(void) {
// Enable counter
	PIT->CHANNEL[1].TCTRL &= ~PIT_TCTRL_TEN_MASK;
}

void PIT_IRQHandler() {
	//static unsigned LCD_update_delay = LCD_UPDATE_PERIOD;
	//clear pending IRQ
	NVIC_ClearPendingIRQ(PIT_IRQn);
	
	// check to see which channel triggered interrupt 
	if (PIT->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK) 
	{
		// clear status flag for timer channel 0
		PIT->CHANNEL[0].TFLG &= PIT_TFLG_TIF_MASK;	
		// Do ISR work
		PIT_interrupt_counter++;
		//Add Handler Code here
		Stop_PIT0();
		FadeOut(SliderPos,0,step_delay);
	} 
	else if (PIT->CHANNEL[1].TFLG & PIT_TFLG_TIF_MASK) 
	{
		// clear status flag for timer channel 1
		PIT->CHANNEL[1].TFLG &= PIT_TFLG_TIF_MASK;
		SET_LED_RED(100);
		DelayMS(100);
		SET_LED_RED(0);
		if(LowBattery==0)
			Stop_PIT1();
	} 
}

// *******************************ARM University Program Copyright © ARM Ltd 2013*************************************   

#include "stdint.h"
#include "project1.h"
#include "app_init.h"
#include "board.h"
#include "TSS_API.h"
#include "timers.h"
#include "ADC.h"

/* Defining the global brightness variable here */
uint8_t SliderPos = 50;
uint16_t step_delay = 20;
uint8_t LED_on = 0;
uint8_t LowBattery = 0;
void FadeIn(int start,int end, int delay)
{
	LowBattWarn_ON();
	Stop_PIT1();
	if (LED_on == 1)
		return;
	else
	{
		int i;
		for (i=start;i<=end;i++) 
		{
			DelayMS(delay);
			SET_LED_RED(i);
			SET_LED_GREEN(i);
			SET_LED_BLUE(i);
		}
		LED_on = 1;
	}
	return;
}

void FadeOut(int start, int end, int delay)
{
	if (LED_on == 0)
		return;
	else
	{
		int i;
		for (i=start;i>=end;i--) 
		{
			DelayMS(delay);
			SET_LED_RED(i);
			SET_LED_GREEN(i);
			SET_LED_BLUE(i);
		}
		LED_on = 0;
	}
	
	if(LowBattery==1)
		Start_PIT1();
	
	return;
}

void SetLEDs(int R, int G, int B)
{
	SET_LED_RED(R);
	SET_LED_GREEN(G);
	SET_LED_BLUE(B);
	return;
}

void LowBattWarn_ON(void)
{
	if(LowBattery == 1 )
	{
		//Stop the OFF state red flashing
		Stop_PIT1();

		//White for 2 seconds
		SET_LED_RED(100);
		SET_LED_GREEN(100);
		SET_LED_BLUE(100);
		DelayMS(2000);
		
		//Red for 250ms
		SET_LED_GREEN(0);
		SET_LED_BLUE(0);
		DelayMS(250);
		
		//Turn Off
		SET_LED_RED(0);
		SET_LED_GREEN(0);
		SET_LED_BLUE(0);
	}
	return;
	}

uint8_t LowBattCheck(void)
{
	LowBattery = Measure_VRail() > 3.2 ? 1:0;
	return LowBattery;
}

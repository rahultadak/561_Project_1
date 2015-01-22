#include "stdint.h"
#include "project1.h"
#include "app_init.h"
#include "board.h"
#include "TSS_API.h"

/* Defining the global brightness variable here */
uint8_t SliderPos = 50;
uint16_t step_delay = 20;
int LED_on = 0;
void FadeIn(int start,int end, int delay)
{
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
		return;
}

void SetLEDs(int R, int G, int B)
{
	SET_LED_RED(R);
	SET_LED_GREEN(G);
	SET_LED_BLUE(B);
	return;
}

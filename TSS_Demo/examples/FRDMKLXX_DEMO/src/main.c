/*HEADER********************************************************************
*
* Copyright (c) 2013 Freescale Semiconductor;
* All Rights Reserved
*
***************************************************************************
*
* THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*
**************************************************************************
*
* Comments:
*
*   This file contains the source for the TSS example program.
*
*END************************************************************************/
#include "TSS_API.h"
#include "freemaster.h"
#include "app_init.h"
#include "events.h"
#include "main.h"
#include "board.h"

#include <math.h>
#include "LEDs.h"
#include "i2c.h"
#include "mma8451.h"
#include "delay.h"

#include "project1.h"
#include "timers.h"
#include "ADC.h"

uint16_t u16LPcounter = 0u;

/*********************** GUI - FreeMASTER TSA table ************************/


#if FMSTR_USE_TSA
  /* Example of FreeMASTER User Application Table */
  FMSTR_TSA_TABLE_BEGIN(UserAppTable)
    FMSTR_TSA_RW_VAR(u16LPcounter, FMSTR_TSA_UINT8)
  FMSTR_TSA_TABLE_END()

  /* FreeMASTER Global Table List definition */
  FMSTR_TSA_TABLE_LIST_BEGIN()
    #if TSS_USE_FREEMASTER_GUI
      FMSTR_TSA_TABLE(TSS_Table)
    #endif
    FMSTR_TSA_TABLE(UserAppTable)
  FMSTR_TSA_TABLE_LIST_END()
#endif

/**
 * \brief  Main function
 *
 * \param  void
 *
 * \return int
 */

uint8_t BoardTilted;
int main (void)
{
  InitPorts();
  /* Default TSS init */
  TSS_Init_ASlider();
  /* Init PWM for LED control */
  PWM_init();
	/* Init Accelerometer */
	i2c_init();																/* init i2c	*/
	if (!init_mma()) {												/* init mma peripheral */
		SET_LED_RED(127);
		//Control_RGB_LEDs(1, 0, 0);							/* Light red error LED */
		while (1)																/* not able to initialize mma */
			;
	}
	
	Init_PIT(119999999,119999999); //TODO Debug set only to 3 seconds, change to 10 seconds
		
	Init_ADC();
	DelayMS(100); //Init Delay
	
	//Initialization complete signal
	SET_LED_GREEN(100);
	DelayMS(200);
	SET_LED_GREEN(0);
	DelayMS(200);
	SET_LED_GREEN(100);
	DelayMS(200);
	SET_LED_GREEN(0);
	DelayMS(200);
	
	//Checking status of battery to start signalling
	if (LowBattCheck() == 1)
		Start_PIT1();	

  #if TSS_USE_FREEMASTER_GUI
    FreeMASTER_Init();
  #endif
  /* Enable Interrupts globally */
  EnableInterrupts();
  /* Reset Low Power Counter flag */
  u16LPcounter = 0u;
	BoardTilted = 0;
  while(1)
  {
		LowBattCheck();		
    if (TSS_Task() == TSS_STATUS_OK)
    {
		}
		/* Calculating angle of the board */
		read_full_xyz();
		convert_xyz_to_roll_pitch();
		//set trigger if angle changes
		if ((roll>33||pitch>33)&&(BoardTilted==0))
		{	
			BoardTilted = 1;
			FadeIn(0,SliderPos,step_delay);
			Start_PIT0();
		}
		else if((roll<=30&&pitch<=30)&&(BoardTilted==1))
		{
			Stop_PIT0();
			//DelayMS(150);
			BoardTilted = 0;
			FadeOut(SliderPos,0,step_delay);
		}

    /* Write your code here ... */
  }
}

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
*   This file contains the init functions.
*
*END************************************************************************/
#include "TSS_API.h"
#include "board.h"
#include "main.h"
#include "wdog.h"
#include "app_init.h"

/**
 * \brief  Global init of the board ports
 *
 * \param  void
 *
 * \return void
 */
void InitPorts(void)
{
  /* Disable WatchDog */
  wdog_disable();
}

/**
 * \brief  Inits TSS for Analog Slider module
 *
 * \param  void
 *
 * \return void
 */
void TSS_Init_ASlider(void)
{
  #if (TSS_USE_AUTO_SENS_CALIBRATION == 0)
    uint8_t lcv;
  #endif

  #ifdef LPTMR_STOP
    LPTMR_STOP;
  #endif

  DelayMS(100); /* For Signal Stabilization */

  /* Initializes the TSS */
  (void)TSS_Init();

  /* Set Number of Samples */
  (void)TSS_SetSystemConfig(System_NSamples_Register, 0x08);

  #if (TSS_USE_AUTO_SENS_CALIBRATION == 0)
    /* Sets the Sensitivity value for each electrode */
    for (lcv =0; lcv < TSS_N_ELECTRODES; lcv++)
    {
      (void)TSS_SetSystemConfig(System_Sensitivity_Register + lcv, 0x1A);
    }
  #endif

  #if (LOW_POWER_TSI == 1)
    /* Low Power Config */
    TSS_SetSystemConfig(System_LowPowerElectrode_Register, 0u);
    TSS_SetSystemConfig(System_LowPowerElectrodeSensitivity_Register, 0x1A);
  #endif

  /* Auto Trigger Config */
  #if (APP_TSS_USE_TRIGGER_MODE == AUTO)
    (void)TSS_SetSystemConfig(System_SystemTrigger_Register, TSS_TRIGGER_MODE_AUTO);   /* TSS_TRIGGER_MODE_AUTO/TSS_TRIGGER_MODE_ALWAYS/TSS_TRIGGER_MODE_SW */
  #elif (APP_TSS_USE_TRIGGER_MODE == ALWAYS)
    (void)TSS_SetSystemConfig(System_SystemTrigger_Register, TSS_TRIGGER_MODE_ALWAYS); /* TSS_TRIGGER_MODE_AUTO/TSS_TRIGGER_MODE_ALWAYS/TSS_TRIGGER_MODE_SW */
  #elif (APP_TSS_USE_TRIGGER_MODE == SW)
    (void)TSS_SetSystemConfig(System_SystemTrigger_Register, TSS_TRIGGER_MODE_SW);   /* TSS_TRIGGER_MODE_AUTO/TSS_TRIGGER_MODE_ALWAYS/TSS_TRIGGER_MODE_SW */
  #endif

  /* Configure the range */
  (void)TSS_SetASliderConfig(cASlider1.ControlId, ASlider_Range_Register, 200);
  /* Configure the TSS to report the touch, release and movement events */
  (void)TSS_SetASliderConfig(cASlider1.ControlId, ASlider_Events_Register, (TSS_ASLIDER_MOVEMENT_EVENT_EN_MASK | TSS_ASLIDER_INITIAL_TOUCH_EVENT_EN_MASK));
  /* Enables Callback function. Enables the control */
  (void)TSS_SetASliderConfig(cASlider1.ControlId, ASlider_ControlConfig_Register, (TSS_ASLIDER_CONTROL_EN_MASK|TSS_ASLIDER_CALLBACK_EN_MASK));

  #if APP_TSS_USE_DCTRACKER
    /* Enables the TSS. Enables the DC Tracking feature. Default DC Tracking value is 0x64 */
  (void)TSS_SetSystemConfig(System_SystemConfig_Register, (TSS_SYSTEM_EN_MASK | TSS_DC_TRACKER_EN_MASK));
  #else
    /* Enables the TSS */
    (void)TSS_SetSystemConfig(System_SystemConfig_Register, (TSS_SYSTEM_EN_MASK));
  #endif

  #ifdef LPTMR_INIT
    LPTMR_INIT;
  #endif
}

/**
 * \brief  Common Delay function in ms units
 *
 * \param  void
 *
 * \return void
 */
void DelayMS(uint16_t u16delay)
{
  uint16_t i;
  uint16_t u16timeout;
  SIM_MemMapPtr sim = SIM_BASE_PTR;

  /* Timer Init */
  sim->SCGC6 |= SIM_SCGC6_TPM1_MASK;  /* TPM1 clock enable */
  sim->SOPT2 |= SIM_SOPT2_TPMSRC(1);  /* 00=Clk.disabled,01=MCGFLLCLK/MCGPLLCLK/2,10 = OSCERCLK,11=MCGIRCLK */
  TPM1_SC = 0u;
  TPM1_CNT = 0x0000U;
  TPM1_SC |= TPM_SC_PS(7);
  TPM1_MOD = ((CORE_CLK_HZ / 128u) / 1000u); /* Calculate Mod for 1ms */
  TPM1_SC |= 0x08U;

  for (i = 0; i < u16delay; i++)
  {
    u16timeout = 0u;
    do
    {
      u16timeout++;
    } while (!(TPM1_SC & TPM_SC_TOF_MASK) && (u16timeout < 0x7FFFu));
    TPM1_SC |= TPM_SC_TOF_MASK;      /* Clear Flag */
    TPM1_CNT = 0x0000U;
  }

  TPM1_SC = 0u;
}

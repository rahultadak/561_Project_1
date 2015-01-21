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
*   This file contains the specific header code on the FRDMKL25 board.
*
*END************************************************************************/
#ifndef BOARD_H
#define BOARD_H

#include "derivative.h"

/* Global defines */
#define DEBUG_PRINT

/* Define the CPU used on this hardware platform */
#define CPU_MKL25Z128LK4

/* Define the clock parameters on this hardware platform */
#define CLK0_FREQ_HZ        8000000
#define CLK0_FREQ_MHZ     8
#define CLK0_TYPE           CRYSTAL
#undef  NO_PLL_INIT
#define PLL0_PRDIV          (CLK0_FREQ_MHZ/2)
#define PLL0_VDIV           24

/* Define the default serial port interface */
#define TERM_PORT           UART0_BASE_PTR
#define TERM_PORT_NUM       0
#define TERMINAL_BAUD       115200
#undef  HW_FLOW_CONTROL


/* Serial Port Info */
#define FMSTR_UART_PORT              UART0_BASE_PTR
#define FMSTR_UART_VECTOR            28
#define FMSTR_UART_PORT_NUM          0
#define FMSTR_UART_BAUD              115200
#define CORE_CLK_HZ                  24000000

/* LED Control Macros */
#define TPM_Cn_MODE        (TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK)
#define TPM_MODULE         255
#define TPM_INIT_VAL       0
#define SET_LED_GREEN(x)   TPM2_C1V = (x)
#define SET_LED_RED(x)     TPM2_C0V = (x)
#define SET_LED_BLUE(x)    TPM0_C1V = (x)

#define LPTMR_STOP LPTMRStop()
#define LPTMR_INIT LPTMRInit()

#define LOW_POWER_TSI   0

#define APP_LPTRM_USE_OSCERKSEL 1

/* Function prototypes */
#ifdef __cplusplus
  extern "C" {
#endif
  void LPTMRStop(void);
  void LPTMRInit(void);
  void LPTMRIsr(void);
  void PWM_init(void);
  void FreeMASTER_Init(void);
  void LowPowerControl(void);
  void InitDevices(void);
#ifdef __cplusplus
  }
#endif

#endif  /* BOARD_H */

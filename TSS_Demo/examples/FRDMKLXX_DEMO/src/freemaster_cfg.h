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
*   FreeMASTER Serial Communication Driver configuration file.
*
*END************************************************************************/
#ifndef FREEMASTER_CFG_H
#define FREEMASTER_CFG_H

#include "board.h"

/******************************************************************************
* Select interrupt or poll-driven serial communication
******************************************************************************/

#define FMSTR_LONG_INTR    0        // complete message processing in interrupt
#define FMSTR_SHORT_INTR   1        // SCI FIFO-queuing done in interrupt
#define FMSTR_POLL_DRIVEN  0        // no interrupt needed, polling only

/*****************************************************************************
* Select communication interface (SCI or CAN)
******************************************************************************/

#define FMSTR_SCI_BASE         FMSTR_UART_PORT    /* Defined in main.h */
#define FMSTR_SCI_INTERRUPT    FMSTR_UART_VECTOR  /* Defined in main.h */

#define FMSTR_CAN_BASE         0x400A4000u     // FlexCAN1 base on K60
#define FMSTR_CAN_RX_INTERRUPT 38   // HC12 only, MSCAN RX interrupt number (use PRM if not defined)
#define FMSTR_CAN_TX_INTERRUPT 39   // HC12 only, MSCAN TX interrupt number (use PRM if not defined)


#define FMSTR_USE_SCI          1    // To selecet SCI communication interface
#define FMSTR_USE_FLEXCAN      0    // To selecet FlexCAN communication interface

/******************************************************************************
* Input/output communication buffer size
******************************************************************************/

#define FMSTR_COMM_BUFFER_SIZE 0    // set to 0 for "automatic"

/******************************************************************************
* Receive FIFO queue size (use with FMSTR_SHORT_INTR only)
******************************************************************************/

#define FMSTR_COMM_RQUEUE_SIZE 32   // set to 0 for "default"

/*****************************************************************************
* Support for Application Commands
******************************************************************************/

#define FMSTR_USE_APPCMD       0    // enable/disable App.Commands support
#define FMSTR_APPCMD_BUFF_SIZE 32   // App.Command data buffer size
#define FMSTR_MAX_APPCMD_CALLS 4    // how many app.cmd callbacks? (0=disable)

/*****************************************************************************
* Oscilloscope support
******************************************************************************/

#define FMSTR_USE_SCOPE       1     // enable/disable scope support
#define FMSTR_MAX_SCOPE_VARS  8     // max. number of scope variables (2..8)

/*****************************************************************************
* Recorder support
******************************************************************************/

#define FMSTR_USE_RECORDER    0     // enable/disable recorder support
#define FMSTR_MAX_REC_VARS    8     // max. number of recorder variables (2..8)
#define FMSTR_REC_OWNBUFF     0     // use user-allocated rec. buffer (1=yes)

// built-in recorder buffer (use when FMSTR_REC_OWNBUFF is 0)
#define FMSTR_REC_BUFF_SIZE   256  // built-in buffer size

// recorder time base, specifies how often the recorder is called in the user app.
#define FMSTR_REC_TIMEBASE    FMSTR_REC_BASE_MILLISEC(0) // 0 = "unknown"

/*****************************************************************************
* Target-side address translation (TSA)
******************************************************************************/

#define FMSTR_USE_TSA         1     // enable TSA functionality
#define FMSTR_USE_TSA_SAFETY  0     // enable access to TSA variables only
#define FMSTR_USE_TSA_INROM   0     // TSA tables declared as const (put to ROM)

/*****************************************************************************
* Enable/Disable read/write memory commands
******************************************************************************/

#define FMSTR_USE_READMEM      1    // enable read memory commands
#define FMSTR_USE_WRITEMEM     1    // enable write memory commands
#define FMSTR_USE_WRITEMEMMASK 1    // enable write memory bits commands

/*****************************************************************************
* Enable/Disable read/write variable commands (a bit faster than Read Mem)
******************************************************************************/

#define FMSTR_USE_READVAR      0    // enable read variable fast commands
#define FMSTR_USE_WRITEVAR     0    // enable write variable fast commands
#define FMSTR_USE_WRITEVARMASK 0    // enable write variable bits fast commands

/*****************************************************************************
* User callback called from FMSTR_Isr() handler
******************************************************************************/

#define FMSTR_ISR_CALLBACK     TSS_FmstrIsrCallbackCustom

#endif /* __FREEMASTER_CFG_H */

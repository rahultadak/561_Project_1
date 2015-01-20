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
*   Main header file.
*
*END************************************************************************/
#ifndef MAIN_H
#define MAIN_H

#include "derivative.h"

/* Multiplexing Macros */

#define MUX_ALT0     0 /* Signal */
#define MUX_ALT1     1 /* GPIO */
#define MUX_ALT2     2
#define MUX_ALT3     3

/* Low Power Types */

#define LP_TYPE_LLS      0x03
#define LP_TYPE_VLPS     0x02
#define LP_TYPE_VLLSX    0x04
#define LOW_POWER_MODE   LP_TYPE_LLS

/* Application Constants */

#define ALWAYS                         1     /* ~1ms  period of the measurement */
#define SW                             2     /* set 20ms  period of the measurement */
#define AUTO                           3     /* set 20ms  period of the measurement */

/* Application Setup */

#define APP_TSS_USE_DCTRACKER          1
#define APP_TSS_USE_TRIGGER_MODE       ALWAYS

#endif

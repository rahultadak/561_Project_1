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
* Comments:
*
*   This file contains electrode and other Touch Sensing System setup parameters
*   GLOBAL (common for the project)
*
*END************************************************************************/
#ifndef TSS_SYSTEM_SETUP_GLOBAL_H
#define TSS_SYSTEM_SETUP_GLOBAL_H

  /* TSS Features Configuration */
  #define TSS_USE_KEYDETECTOR_VERSION            1
#if (TSS_USE_KEYDETECTOR_VERSION == 2)
  #define TSS_USE_AFID_FAST_FILTER_RATIO             25  /* Ratio is fsample/fcutoff */

  #define TSS_USE_AFID_SLOW_FILTER_RATIO             100 /* Ratio is fsample/fcutoff */
#endif
  #define TSS_USE_NOISE_MODE                     0

  #define TSS_USE_NOISE_SIGNAL_LOG               0

  #define TSS_USE_DELTA_LOG                      1

  #define TSS_USE_SIGNAL_LOG                     1

  #define TSS_USE_INTEGRATION_DELTA_LOG          1

  #define TSS_USE_GPIO_STRENGTH                  0

  #define TSS_USE_GPIO_SLEW_RATE                 0

  #define TSS_USE_IIR_FILTER                     1

  #define TSS_USE_NOISE_AMPLITUDE_FILTER         0

  #define TSS_USE_TRIGGER_FUNCTION               1

  #define TSS_USE_LOWPOWER_THRESHOLD_BASELINE    1

  #define TSS_USE_AUTO_SENS_CALIBRATION          1

  #define TSS_USE_AUTO_SENS_CALIB_INIT_DURATION  100

  #define TSS_USE_AUTO_SENS_CALIB_LOW_LIMIT      0

  #define TSS_USE_BASELINE_INIT_DURATION         15

  #define TSS_USE_DATA_CORRUPTION_CHECK          1

  #define TSS_USE_FREEMASTER_GUI                 0

  #define TSS_USE_SIGNAL_SHIELDING               0

  #define TSS_USE_SIGNAL_DIVIDER                 1

  #define TSS_USE_SIGNAL_MULTIPLIER              0

  #define TSS_USE_STUCK_KEY                      1

  #define TSS_USE_NEGATIVE_BASELINE_DROP         1

  #define TSS_USE_AUTO_HW_RECALIBRATION          1

  #define TSS_USE_CONTROL_PRIVATE_DATA           0

  #define TSS_ENABLE_DIAGNOSTIC_MESSAGES         0

  #define TSS_ONFAULT_CALLBACK                   TSS_fOnFault /* Identifier of the user's OnFault callback */

  #define TSS_ONINIT_CALLBACK                    TSS_fOnInit /* Identifier of the user's OnInit callback */

  #define TSS_N_ELECTRODES                       2       /* Number of electrodes present in the system */

#endif

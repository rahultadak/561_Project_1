/*HEADER**********************************************************************
 *
 * Copyright 2013 Freescale Semiconductor, Inc.
 *
 * Freescale Confidential and Proprietary - use of this software is
 * governed by the Freescale TSS License distributed with this
 * material. See the license file distributed for more
 * details.
 *
 *****************************************************************************
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
 *****************************************************************************
 *
 * Comments:
 *
 *   This file contains constants for all objects in the wrapper.
 *
 *END************************************************************************/
#ifndef TSS_CPP_CONST_H
#define TSS_CPP_CONST_H

namespace tss
{

  /** TSS events for TSSSystem
   */
  enum TSSSystemEvent
  {
    FAULT = 0,      /* only one common callback available in TSSSystem */
    CHARGE_TIMEOUT_FAULT = 1,
    SMALL_CAPACITOR_FAULT = 2,
    SMALL_TRIGGER_FAULT = 3,
    DATA_CORRUPTION_FAULT = 4,
    SYSTEM_INITIALIZATION_FAULT = 5,
    SYSTEM_FAULT = 255,
  };

  /** Control types avaliable in TSS
   */
  enum TSSControlTypes
  {
    TSS_KEYPAD = 1,
    TSS_SLIDER = 2,
    TSS_ROTARY = 3,
    TSS_ASLIDER = 4,
    TSS_AROTARY = 5,
    TSS_MATRIX = 6
  };

  /** TSS System enable/disable flags
   */
  enum TSSSystemFlag
  {
    TSS_SYSTEM = 15,
    TSS_SWI = 14,
    TSS_DC_TRACK = 13,
    TSS_STUCK_KEY = 12,
    TSS_WATER_TOLERANCE = 8,
    TSS_PROXIMITY = 7,
    TSS_LOW_POWER = 6,
  };

  /** TSS System set/get flags
   */
  enum TSSSystemConfig
  {
    TSS_FAULTS = 0,
    TSS_SYSTEM_CONFIG = 1,
    TSS_NSAMPLES = 3,
    TSS_DC_TRACKER_RATE = 4,
    TSS_SLOW_DC_TRACKER_FACTOR = 5,
    TSS_RESPONSE_TIME = 6,
    TSS_STUCK_KEY_TIMEOUT = 7,
    TSS_LP_ELECTRODE = 8,        /* low power electrode */
    TSS_LP_ELEC_SENSITIVITY = 9, /* low power sensitivity */
    TSS_TRIGGER = 10
  };

  /** Control's enable/disable offset
   */
  enum TSSControlEnOffset
  {
    TSS_CONTROL_EN_SHIFT = 256,   /* all controls */
    TSS_MATRIX_SHIFT = 128,       /* only matrix */
    TSS_POSITION_DEC_SHIFT = 64,  /* position decoders */
    TSS_KEYPEAD_SHIFT = 32        /* only keypad */
  };

  /** Control's set/get offset
   */
  enum TSSControlSetOffset
  {
    TSS_KEYPAD_SET_SHIFT = 512,       /* only for keypad */
    TSS_CONTROL_SET_SHIFT = 256,      /* all controls */
    TSS_POSITION_DEC_SET_SHIFT = 128, /* position decoders */
    TSS_ANALOG_SET_SHIFT = 64,        /* analog decoders */
    TSS_MATRIX_SET_SHIFT = 32,        /* only for matrix */
    TSS_READ_ONLY_SHIFT = 1024        /* read only flag */
  };

  /** Control enable/disable flags
   */
  enum TSSControlFlag
  {
    /* Control - Aval for all controls */
    TSS_CONTROL = 7 + TSS_CONTROL_EN_SHIFT,
    TSS_CALLBACK = 6 + TSS_CONTROL_EN_SHIFT,
    /* Matrix */
    TSS_GESTURES = 5 + TSS_MATRIX_SHIFT,
    /* Position decoders (Aslider,Arotary,Slider,Rotary) */
    TSS_ALL_RELEASE = 4 + TSS_POSITION_DEC_SHIFT,
    TSS_HOLD_AUTOREPEAT = 3 + TSS_POSITION_DEC_SHIFT,
    TSS_HOLD = 2 + TSS_POSITION_DEC_SHIFT,
    TSS_MOVEMENT = 1 + TSS_POSITION_DEC_SHIFT,
    TSS_INITIAL_TOUCH  = 0 + TSS_POSITION_DEC_SHIFT,
    /* Keypad */
    TSS_KEY_EXCEEDED = 4 + TSS_KEYPEAD_SHIFT,
    TSS_BUFFER_OVERFLOW = 3 + TSS_KEYPEAD_SHIFT,
    TSS_AUTOREPEAT = 2 + TSS_KEYPEAD_SHIFT,
    TSS_RELEASE = 1 + TSS_KEYPEAD_SHIFT,
    TSS_TOUCH = 0 + TSS_KEYPEAD_SHIFT
  };

  /** Control set/get flags
   */
  enum TSSControlConfig
  {
    /* Keypad */
    TSS_MAXTOUCHES_RATE_RW = 6 + TSS_KEYPAD_SET_SHIFT,
    TSS_AUTOREPEAT_START_RW = 7 + TSS_KEYPAD_SET_SHIFT,
    /* All controls */
    TSS_AUTOREPEAT_RATE_RW = 5 + TSS_CONTROL_SET_SHIFT,
    /* Position decoders (Aslider,Arotary,Slider,Rotary) */
    TSS_MOVEMENT_TIMEOUT_RW = 6 + TSS_POSITION_DEC_SET_SHIFT,
    /* Analog controls (ASlider, ARotary) */
    TSS_RANGE_RW = 7 + TSS_ANALOG_SET_SHIFT,
    /* Matrix */
    TSS_RANGE_X_RW = 11 + TSS_MATRIX_SET_SHIFT,
    TSS_RANGE_Y_RW = 12 + TSS_MATRIX_SET_SHIFT,

    /* Read only (get) flags - only position decoders */
    TSS_POSITION_RO = 0 + TSS_POSITION_DEC_SET_SHIFT + TSS_READ_ONLY_SHIFT,
    TSS_MOVEMENT_RO = 1 + TSS_POSITION_DEC_SET_SHIFT + TSS_READ_ONLY_SHIFT,
    TSS_DIRECTION_RO = 2 + TSS_POSITION_DEC_SET_SHIFT + TSS_READ_ONLY_SHIFT,
    TSS_DISPLACEMENT_RO = 3 + TSS_POSITION_DEC_SET_SHIFT + TSS_READ_ONLY_SHIFT,
    TSS_TOUCH_RO = 4 + TSS_POSITION_DEC_SET_SHIFT + TSS_READ_ONLY_SHIFT,
    TSS_INVALID_POSITION_RO = 5 + TSS_POSITION_DEC_SET_SHIFT + TSS_READ_ONLY_SHIFT,

    /* Read only flags available only for Matrix */
    TSS_POSITION_X_RO = 6 + TSS_MATRIX_SET_SHIFT + TSS_READ_ONLY_SHIFT,
    TSS_POSITION_Y_RO = 7 + TSS_MATRIX_SET_SHIFT + TSS_READ_ONLY_SHIFT,
    TSS_GESTURE_DISTANCE_X_RO = 8 + TSS_MATRIX_SET_SHIFT + TSS_READ_ONLY_SHIFT,
    TSS_GESTURE_DISTANCE_Y_RO = 9 + TSS_MATRIX_SET_SHIFT + TSS_READ_ONLY_SHIFT,
    TSS_GESTURE_RO = 10 + TSS_MATRIX_SET_SHIFT + TSS_READ_ONLY_SHIFT
  };

}

#endif

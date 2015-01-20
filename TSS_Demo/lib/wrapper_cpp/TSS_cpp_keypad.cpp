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
 *   This file contains TSSKeypad control implementation.
 *
 *END************************************************************************/
#include "TSS_cpp.h"

namespace tss {

TSSKeypad::TSSKeypad(uint8_t control_num)
  : _callbacks()
{
  TSS_SetControlPrivateData(control_num, static_cast<void *>(this));
  setControlStruct(static_cast<TSS_CSKeypad*>(TSS_GetControlStruct(control_num)));

  /* Explicit call, no virtual inside ctor */
  TSSKeypad::enable(TSS_TOUCH);
  TSSKeypad::enable(TSS_RELEASE);
  TSSKeypad::enable(TSS_CONTROL);
  TSSKeypad::enable(TSS_CALLBACK);
}

TSSKeypad::~TSSKeypad()
{
  TSS_CSKeypad const *keypad_struct = getControlStruct();

  TSS_SetControlPrivateData(keypad_struct->ControlId.ControlNumber, NULL);
  TSSKeypad::disable(TSS_CONTROL);
}

void TSSKeypad::callbackControl(void)
{
  TSS_CSKeypad const *temp = getControlStruct();
  TSS_CSKeypad *keypad_struct = const_cast<TSS_CSKeypad *>(temp); /* cast const away -> keypad's buffer */

  while (!TSS_KEYPAD_BUFFER_EMPTY(*keypad_struct))
  {
    uint8_t event;

    TSS_KEYPAD_BUFFER_READ(event, *keypad_struct);

    if (event & TSS_KEYPAD_BUFFER_RELEASE_FLAG)
    {
      if (_callbacks[KEYPAD_RELEASE] != (fKeypadCallback)NULL)
      {
        _callbacks[KEYPAD_RELEASE](this, KEYPAD_RELEASE, event & 0x7F);
      }
      onRelease(event & 0x7F);
    }
    else
    {
      if (_callbacks[KEYPAD_TOUCH] != (fKeypadCallback)NULL)
      {
        _callbacks[KEYPAD_TOUCH](this, KEYPAD_TOUCH, event);
      }
      onTouch(event);
    }
  }

  if (keypad_struct->Events.MaxKeysFlag != 0u)
  {
    if (_callbacks[KEYPAD_KEYS_EXCEEDED] != (fKeypadCallback)NULL)
    {
      _callbacks[KEYPAD_KEYS_EXCEEDED](this, KEYPAD_KEYS_EXCEEDED, keypad_struct->MaxTouches);
    }
    onExceededKeys(keypad_struct->MaxTouches);
  }
}

bool TSSKeypad::enable(TSSControlFlag flag)
{
  if (flag & (TSS_CONTROL_EN_SHIFT | TSS_KEYPEAD_SHIFT))
  {
    uint8_t enable_flag;
    uint16_t config_register;
    uint8_t  keypad_event;
    TSS_CSKeypad const *keypad_struct = getControlStruct();

    enable_flag = (uint8_t)(flag & 0x1F);
   (enable_flag > 5u) ? keypad_event = 1u : keypad_event = 4u;
    config_register = TSS_GetKeypadConfig(keypad_struct->ControlId, keypad_event);
    config_register |= (uint16_t)(1 << enable_flag);
    if (TSS_SetKeypadConfig(keypad_struct->ControlId, keypad_event, config_register) == TSS_STATUS_OK)
    {
      return true;
    }
  }
  return false;
}

bool TSSKeypad::disable(TSSControlFlag flag)
{
  if (flag & (TSS_CONTROL_EN_SHIFT | TSS_KEYPEAD_SHIFT))
  {
    uint8_t disable_flag;
    uint16_t config_register;
    uint8_t  keypad_event;
    TSS_CSKeypad const *keypad_struct = getControlStruct();

    disable_flag = (uint16_t)(flag & 0x1F);
    (disable_flag > 5u) ? keypad_event = 1u : keypad_event = 4u; /* control config/event config */
    config_register = TSS_GetKeypadConfig(keypad_struct->ControlId, keypad_event);
    config_register &= (uint16_t)~(1u << disable_flag);
    if (TSS_SetKeypadConfig(keypad_struct->ControlId, keypad_event, config_register) == TSS_STATUS_OK)
    {
      return true;
    }
  }
  return false;
}

/* Internal get/set event function */
static uint8_t keypad_get_set_event(uint16_t event_flag)
{
  if (event_flag == TSS_MAXTOUCHES_RATE_RW)
  {
    return Keypad_MaxTouches_Register;
  }
  else if (event_flag == TSS_AUTOREPEAT_RATE_RW)
  {
    return Keypad_AutoRepeatRate_Register;
  }
  else
  {
    return (uint8_t)(event_flag & 0x1F);
  }
}

bool TSSKeypad::set(TSSControlConfig config, uint8_t value)
{
  /* only RW flags can be set */
  if (!(config & (TSSControlConfig)TSS_READ_ONLY_SHIFT) && (config & (TSS_KEYPAD_SET_SHIFT | TSS_CONTROL_SET_SHIFT)))
  {
    uint8_t set_flag;
    TSS_CSKeypad const *keypad_struct = getControlStruct();

    set_flag = keypad_get_set_event(config);
    if (TSS_SetKeypadConfig(keypad_struct->ControlId, set_flag , value) == TSS_STATUS_OK)
    {
      return true;
    }
  }

  return false;
}

uint16_t TSSKeypad::get(TSSControlConfig config) const
{
  if (config & (TSS_KEYPAD_SET_SHIFT | TSS_CONTROL_SET_SHIFT))
  {
    uint8_t get_flag;
    TSS_CSKeypad const *keypad_struct = getControlStruct();

    get_flag = keypad_get_set_event(config);
    return TSS_GetKeypadConfig(keypad_struct->ControlId, get_flag);
  }
  return TSS_ERROR_GETCONF_ILLEGAL_PARAMETER;
}

void TSSKeypad::onTouch(uint8_t elec_num)
{

}

void TSSKeypad::onRelease(uint8_t elec_num)
{

}

void TSSKeypad::onExceededKeys(uint8_t max_touches)
{

}

} /* end of tss namespace */

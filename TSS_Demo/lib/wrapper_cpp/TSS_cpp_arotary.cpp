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
 *   This file contains TSSARotary control implementation.
 *
 *END************************************************************************/
#include "TSS_cpp.h"

namespace tss {

TSSARotary::TSSARotary(UINT8 control_num)
  : _callbacks()
{
  TSS_SetControlPrivateData(control_num, static_cast<void *>(this));
  setControlStruct(static_cast<TSS_CSARotary*>(TSS_GetControlStruct(control_num)));

  /* Explicit call, no virtual inside ctor */
  TSSARotary::enable(TSS_MOVEMENT);
  TSSARotary::enable(TSS_CONTROL);
  TSSARotary::enable(TSS_CALLBACK);
}

TSSARotary::~TSSARotary()
{
  TSS_CSARotary const *arotary_struct = getControlStruct();

  TSS_SetControlPrivateData(arotary_struct->ControlId.ControlNumber, NULL);  /* erase this pointer from TSS */
  TSSARotary::disable(TSS_CONTROL);
}

void TSSARotary::callbackControl(void)
{
  TSS_CSARotary const *arotary_struct = getControlStruct();

  if (arotary_struct->DynamicStatus.Movement == 1)
  {
    if (_callbacks[AROTARY_MOVEMENT] != (fARotaryCallback)NULL)
    {
      _callbacks[AROTARY_MOVEMENT](this, AROTARY_MOVEMENT, arotary_struct->Position);
    }
    onMovement(arotary_struct->Position);
  }
  else if (arotary_struct->Events.Touch != 0u)
  {
    if (_callbacks[AROTARY_INITIAL_TOUCH] != (fARotaryCallback)NULL)
    {
      _callbacks[AROTARY_INITIAL_TOUCH](this, AROTARY_INITIAL_TOUCH, arotary_struct->Position);
    }
    onInitialTouch(arotary_struct->Position);
  }
  else if (arotary_struct->Events.Touch == 0u)
  {
    if (_callbacks[AROTARY_ALL_RELEASE] != (fARotaryCallback)NULL)
    {
      _callbacks[AROTARY_ALL_RELEASE](this, AROTARY_ALL_RELEASE, arotary_struct->Position);
    }
    onAllRelease(arotary_struct->Position);
  }
}

bool TSSARotary::enable(TSSControlFlag flag)
{
  if (flag & (TSS_CONTROL_EN_SHIFT | TSS_POSITION_DEC_SHIFT))
  {
    uint8_t enable_flag;
    uint16_t config_register;
    uint8_t  arotary_event;
    TSS_CSARotary const *arotary_struct = getControlStruct();

    enable_flag = (uint8_t)(flag & 0x1Fu);
    (enable_flag > 4u) ? arotary_event = 1u : arotary_event = 4u;
    config_register = TSS_GetARotaryConfig(arotary_struct->ControlId, arotary_event);
    config_register |= (uint16_t)(1 << enable_flag);
    if (TSS_SetARotaryConfig(arotary_struct->ControlId, arotary_event, config_register) == TSS_STATUS_OK)
    {
      return true;
    }
  }
  return false;
}

bool TSSARotary::disable(TSSControlFlag flag)
{
  if (flag & (TSS_CONTROL_EN_SHIFT | TSS_POSITION_DEC_SHIFT))
  {
    uint8_t disable_flag;
    uint16_t config_register;
    uint8_t  arotary_event;
    TSS_CSARotary const *arotary_struct = getControlStruct();

    disable_flag = (uint16_t)(flag & 0x1Fu);
    (disable_flag > 4u) ? arotary_event = 1u : arotary_event = 4u; /* control config/event config */
    config_register = TSS_GetARotaryConfig(arotary_struct->ControlId, arotary_event);
    config_register &= (uint16_t)~(1u << disable_flag);
    if (TSS_SetARotaryConfig(arotary_struct->ControlId, arotary_event, config_register) == TSS_STATUS_OK)
    {
      return true;
    }
  }
  return false;
}

bool TSSARotary::set(TSSControlConfig config, uint8_t value)
{
  /* only RW flags can be set */
  if (!(config & (TSSControlConfig)TSS_READ_ONLY_SHIFT) && (config & (TSS_CONTROL_SET_SHIFT | TSS_POSITION_DEC_SET_SHIFT | TSS_ANALOG_SET_SHIFT)))
  {
    uint8_t set_flag;
    TSS_CSARotary const *arotary_struct = getControlStruct();

    set_flag = (uint8_t)(config & 0x1Fu);
    if (TSS_SetARotaryConfig(arotary_struct->ControlId, set_flag , value) == TSS_STATUS_OK)
    {
      return true;
    }
  }
  return false;
}

/* Internal get event function */
static uint16_t arotary_get_internal(uint16_t event_flag, TSS_CSARotary const *arotary_struct)
{
  uint16_t get_flag = TSS_ERROR_GETCONF_ILLEGAL_PARAMETER;

  if (event_flag & TSS_READ_ONLY_SHIFT)
  {
    event_flag = (uint16_t)(event_flag & 0x1Fu);

    switch(event_flag)
    {
      case 0:
        get_flag = arotary_struct->Position;
        break;
      case 1:
        get_flag = arotary_struct->DynamicStatus.Movement;
        break;
      case 2:
        get_flag = arotary_struct->DynamicStatus.Direction;
        break;
      case 3:
        get_flag = arotary_struct->DynamicStatus.Displacement;
        break;
      case 4:
        get_flag = arotary_struct->Events.Touch;
        break;
      case 5:
        get_flag = arotary_struct->Events.InvalidPos;
        break;
    }
  }
  else
  {
    event_flag = (uint16_t)(event_flag & 0x1Fu);
    get_flag = TSS_GetARotaryConfig(arotary_struct->ControlId, (uint8_t)event_flag);
  }

  return  get_flag;
}

uint16_t TSSARotary::get(TSSControlConfig config) const
{
  if (config & (TSS_CONTROL_SET_SHIFT | TSS_POSITION_DEC_SET_SHIFT | TSS_ANALOG_SET_SHIFT))
  {
    TSS_CSARotary const *arotary_struct = getControlStruct();

    return arotary_get_internal(config, arotary_struct);
  }
  return TSS_ERROR_GETCONF_ILLEGAL_PARAMETER;
}

void TSSARotary::onInitialTouch(uint8_t position)
{

}

void TSSARotary::onAllRelease(uint8_t position)
{

}

void TSSARotary::onMovement(uint8_t position)
{

}

} /* end of tss namespace */

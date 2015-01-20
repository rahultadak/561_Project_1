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
 *   This file contains TSSRotary control implementation.
 *
 *END************************************************************************/
#include "TSS_cpp.h"

namespace tss {

TSSRotary::TSSRotary(UINT8 control_num)
  : _callbacks()
{
  TSS_SetControlPrivateData(control_num, (void *)this);
  setControlStruct(static_cast<TSS_CSRotary*>(TSS_GetControlStruct(control_num)));

  /* Explicit call, no virtual inside ctor */
  TSSRotary::enable(TSS_MOVEMENT);
  TSSRotary::enable(TSS_CONTROL);
  TSSRotary::enable(TSS_CALLBACK);
}

TSSRotary::~TSSRotary()
{
  TSS_CSRotary const *rotary_struct = getControlStruct();

  TSS_SetControlPrivateData(rotary_struct->ControlId.ControlNumber, NULL);
  TSSRotary::disable(TSS_CONTROL);
}

void TSSRotary::callbackControl(void)
{
  TSS_CSRotary const *rotary_struct = getControlStruct();

  if (rotary_struct->DynamicStatus.Movement == 1)
  {
    if (_callbacks[ROTARY_MOVEMENT] != (fRotaryCallback)NULL)
    {
      _callbacks[ROTARY_MOVEMENT](this, ROTARY_MOVEMENT, rotary_struct->StaticStatus.Position);
    }
    onMovement(rotary_struct->StaticStatus.Position);
  }
  else if (rotary_struct->StaticStatus.Touch != 0u)
  {
    if (_callbacks[ROTARY_INITIAL_TOUCH] != (fRotaryCallback)NULL)
    {
      _callbacks[ROTARY_INITIAL_TOUCH](this, ROTARY_INITIAL_TOUCH, rotary_struct->StaticStatus.Position);
    }
    onInitialTouch(rotary_struct->StaticStatus.Position);
  }
  else if (rotary_struct->StaticStatus.Touch == 0u)
  {
    if (_callbacks[ROTARY_ALL_RELEASE] != (fRotaryCallback)NULL)
    {
      _callbacks[ROTARY_ALL_RELEASE](this, ROTARY_ALL_RELEASE, rotary_struct->StaticStatus.Position);
    }
    onAllRelease(rotary_struct->StaticStatus.Position);
  }
}

bool TSSRotary::enable(TSSControlFlag flag)
{
  if (flag & (TSS_CONTROL_EN_SHIFT | TSS_POSITION_DEC_SHIFT))
  {
    uint8_t enable_flag;
    uint16_t config_register;
    uint8_t  rotary_event;
    TSS_CSRotary const *rotary_struct = getControlStruct();

    enable_flag = (uint8_t)(flag & 0x1F);
    (enable_flag > 4) ? rotary_event = 1 : rotary_event = 4;
    config_register = TSS_GetRotaryConfig(rotary_struct->ControlId, rotary_event);
    config_register |= (uint16_t)(1 << enable_flag);
    if (TSS_SetRotaryConfig(rotary_struct->ControlId, rotary_event, config_register) == TSS_STATUS_OK)
    {
      return true;
    }
  }
  return false;
}

bool TSSRotary::disable(TSSControlFlag flag)
{
  if (flag & (TSS_CONTROL_EN_SHIFT | TSS_POSITION_DEC_SHIFT))
  {
    uint8_t disable_flag;
    uint16_t config_register;
    uint8_t  rotary_event;
    TSS_CSRotary const *rotary_struct = getControlStruct();

    disable_flag = (uint16_t)(flag & 0x1F);
    (disable_flag > 4) ? rotary_event = 1 : rotary_event = 4; /* control config/event config */
    config_register = TSS_GetRotaryConfig(rotary_struct->ControlId, rotary_event);
    config_register &= (uint16_t)~(1u << disable_flag);
    if (TSS_SetRotaryConfig(rotary_struct->ControlId, rotary_event, config_register) == TSS_STATUS_OK)
    {
      return true;
    }
  }
  return false;
}

bool TSSRotary::set(TSSControlConfig config, uint8_t value)
{
  if (!(config & (TSSControlConfig)TSS_READ_ONLY_SHIFT) && (config & (TSS_CONTROL_SET_SHIFT | TSS_POSITION_DEC_SET_SHIFT)))
  {
    uint8_t set_flag;
    TSS_CSRotary const *rotary_struct = getControlStruct();

    set_flag = (uint8_t)(config & 0x1F);
    if (TSS_SetRotaryConfig(rotary_struct->ControlId, set_flag , value) == TSS_STATUS_OK)
    {
      return true;
    }
  }
  return false;
}

/* Internal get event function */
static uint16_t rotary_get_internal(uint16_t event_flag, TSS_CSRotary const *rotary_struct)
{
  uint16_t get_flag = TSS_ERROR_GETCONF_ILLEGAL_PARAMETER;

  if (event_flag & TSS_READ_ONLY_SHIFT)
  {
    event_flag = (uint16_t)(event_flag & 0x1Fu);

    switch(event_flag)
    {
      case 0:
        get_flag = rotary_struct->StaticStatus.Position;
        break;
      case 1:
        get_flag = rotary_struct->DynamicStatus.Movement;
        break;
      case 2:
        get_flag = rotary_struct->DynamicStatus.Direction;
        break;
      case 3:
        get_flag = rotary_struct->DynamicStatus.Displacement;
        break;
      case 4:
        get_flag = rotary_struct->StaticStatus.Touch;
        break;
      case 5:
        get_flag = rotary_struct->StaticStatus.InvalidPos;
        break;
    }
  }
  else
  {
    event_flag = (uint16_t)(event_flag & 0x1Fu);
    get_flag = TSS_GetRotaryConfig(rotary_struct->ControlId, (uint8_t)event_flag);
  }

  return  get_flag;
}

uint16_t TSSRotary::get(TSSControlConfig config) const
{
  if (config & (TSS_CONTROL_SET_SHIFT | TSS_POSITION_DEC_SET_SHIFT))
  {
    TSS_CSRotary const *rotary_struct = getControlStruct();

    return rotary_get_internal(config, rotary_struct);
  }
  return TSS_ERROR_GETCONF_ILLEGAL_PARAMETER;
}

void TSSRotary::onInitialTouch(uint8_t position)
{

}

void TSSRotary::onAllRelease(uint8_t position)
{

}

void TSSRotary::onMovement(uint8_t position)
{

}

} /* end of tss namespace */

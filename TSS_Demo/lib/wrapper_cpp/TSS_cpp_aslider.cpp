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
 *   This file contains TSSASlider control implementation.
 *
 *END************************************************************************/
#include "TSS_API.h"
#include "TSS_cpp.h"

namespace tss {

TSSASlider::TSSASlider(UINT8 control_num)
  : _callbacks()
{
  TSS_SetControlPrivateData(control_num, static_cast<void *>(this));
  setControlStruct(static_cast<TSS_CSASlider*>(TSS_GetControlStruct(control_num)));

  /* Explicit call, no virtual inside ctor */
  TSSASlider::enable(TSS_MOVEMENT);
  TSSASlider::enable(TSS_CONTROL);
  TSSASlider::enable(TSS_CALLBACK);
}

TSSASlider::~TSSASlider()
{
  TSS_CSASlider const *aslider_struct = getControlStruct();

  TSS_SetControlPrivateData(aslider_struct->ControlId.ControlNumber, NULL);
  TSSASlider::disable(TSS_CONTROL);
}

void TSSASlider::callbackControl(void)
{
  TSS_CSASlider const *aslider_struct = (TSS_CSASlider *)getControlStruct();

  if (aslider_struct->DynamicStatus.Movement == 1)
  {
    if (_callbacks[ASLIDER_MOVEMENT] != (fASliderCallback)NULL)
    {
      _callbacks[ASLIDER_MOVEMENT](this, ASLIDER_MOVEMENT, aslider_struct->Position);
    }
    onMovement(aslider_struct->Position);
  }
  else if (aslider_struct->Events.Touch != 0u)
  {
    if (_callbacks[ASLIDER_INITIAL_TOUCH] != (fASliderCallback)NULL)
    {
      _callbacks[ASLIDER_INITIAL_TOUCH](this, ASLIDER_INITIAL_TOUCH, aslider_struct->Position);
    }
    onInitialTouch(aslider_struct->Position);
  }
  else if (aslider_struct->Events.Touch == 0u)
  {
    if (_callbacks[ASLIDER_ALL_RELEASE] != (fASliderCallback)NULL)
    {
      _callbacks[ASLIDER_ALL_RELEASE](this, ASLIDER_ALL_RELEASE, 0);
    }
    onAllRelease(aslider_struct->Position);
  }
}

bool TSSASlider::enable(TSSControlFlag flag)
{
  if (flag & (TSS_CONTROL_EN_SHIFT | TSS_POSITION_DEC_SHIFT))
  {
    uint8_t enable_flag;
    uint16_t config_register;
    uint8_t  aslider_event;
    TSS_CSASlider const *aslider_struct = getControlStruct();

    enable_flag = (uint8_t)(flag & 0x1F);
    (enable_flag > 4) ? aslider_event = 1 : aslider_event = 4;
    config_register = TSS_GetASliderConfig(aslider_struct->ControlId, aslider_event);
    config_register |= (uint16_t)(1 << enable_flag);
    if (TSS_SetASliderConfig(aslider_struct->ControlId, aslider_event, config_register) == TSS_STATUS_OK)
    {
      return true;
    }
  }
  return false;
}

bool TSSASlider::disable(TSSControlFlag flag)
{
  if (flag & (TSS_CONTROL_EN_SHIFT | TSS_POSITION_DEC_SHIFT))
  {
    uint8_t disable_flag;
    uint16_t config_register;
    uint8_t  aslider_event;
    TSS_CSASlider const *aslider_struct = getControlStruct();

    disable_flag = (uint16_t)(flag & 0x1F);
    (disable_flag > 4) ? aslider_event = 1 : aslider_event = 4; /* control config/event config */
    config_register = TSS_GetASliderConfig(aslider_struct->ControlId, aslider_event);
    config_register &= (uint16_t)~(1u << disable_flag);
    if (TSS_SetASliderConfig(aslider_struct->ControlId, aslider_event, config_register) == TSS_STATUS_OK)
    {
      return true;
    }
  }
  return false;
}

bool TSSASlider::set(TSSControlConfig config, uint8_t value)
{
  /* only RW flags can be set */
  if (!(config & (TSSControlConfig)TSS_READ_ONLY_SHIFT) && (config & (TSS_CONTROL_SET_SHIFT | TSS_POSITION_DEC_SET_SHIFT | TSS_ANALOG_SET_SHIFT)))
  {
    uint8_t set_flag;
    TSS_CSASlider const *aslider_struct = getControlStruct();

    set_flag = (uint8_t)(config & 0x1F);
    if (TSS_SetASliderConfig(aslider_struct->ControlId, set_flag, value) == TSS_STATUS_OK)
    {
      return true;
    }
  }
  return false;
}

/* Internal get event function */
static uint16_t aslider_get_internal(uint16_t event_flag, TSS_CSASlider const *aslider_struct)
{
  uint16_t get_flag = TSS_ERROR_GETCONF_ILLEGAL_PARAMETER;

  if (event_flag & TSS_READ_ONLY_SHIFT)
  {
    event_flag = (uint16_t)(event_flag & 0x1Fu);

    switch(event_flag)
    {
      case 0:
        get_flag = aslider_struct->Position;
        break;
      case 1:
        get_flag = aslider_struct->DynamicStatus.Movement;
        break;
      case 2:
        get_flag = aslider_struct->DynamicStatus.Direction;
        break;
      case 3:
        get_flag = aslider_struct->DynamicStatus.Displacement;
        break;
      case 4:
        get_flag = aslider_struct->Events.Touch;
        break;
      case 5:
        get_flag = aslider_struct->Events.InvalidPos;
        break;
    }
  }
  else
  {
    event_flag = (uint16_t)(event_flag & 0x1Fu);
    get_flag = TSS_GetASliderConfig(aslider_struct->ControlId, (uint8_t)event_flag);
  }

  return  get_flag;
}

UINT16 TSSASlider::get(TSSControlConfig config) const
{
  if (config & (TSS_CONTROL_SET_SHIFT | TSS_POSITION_DEC_SET_SHIFT | TSS_ANALOG_SET_SHIFT))
  {
    TSS_CSASlider const *aslider_struct = getControlStruct();

    return aslider_get_internal(config, aslider_struct);
  }
  return TSS_ERROR_GETCONF_ILLEGAL_PARAMETER;
}

void TSSASlider::onInitialTouch(uint8_t position)
{

}

void TSSASlider::onAllRelease(uint8_t position)
{

}

void TSSASlider::onMovement(uint8_t position)
{

}

}

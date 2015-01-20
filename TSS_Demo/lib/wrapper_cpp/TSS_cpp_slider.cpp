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
 *   This file contains TSSSlider control implementation.
 *
 *END************************************************************************/
#include "TSS_cpp.h"

namespace tss {

TSSSlider::TSSSlider(UINT8 control_num)
  : _callbacks()
{
  TSS_SetControlPrivateData(control_num, (void *)this);
  setControlStruct(static_cast<TSS_CSSlider*>(TSS_GetControlStruct(control_num)));

  /* Explicit call, no virtual inside ctor */
  TSSSlider::enable(TSS_MOVEMENT);
  TSSSlider::enable(TSS_CONTROL);
  TSSSlider::enable(TSS_CALLBACK);
}

TSSSlider::~TSSSlider()
{
  TSS_CSSlider const *slider_struct = getControlStruct();

  TSS_SetControlPrivateData(slider_struct->ControlId.ControlNumber, NULL);
  TSSSlider::disable(TSS_CONTROL);
}

void TSSSlider::callbackControl(void)
{
  TSS_CSSlider const *slider_struct = getControlStruct();

  if (slider_struct->DynamicStatus.Movement == 1)
  {
    if (_callbacks[SLIDER_MOVEMENT] != (fSliderCallback)NULL)
    {
      _callbacks[SLIDER_MOVEMENT](this, SLIDER_MOVEMENT, slider_struct->StaticStatus.Position);
    }
    onMovement(slider_struct->StaticStatus.Position);
  }
  else if (slider_struct->StaticStatus.Touch != 0u)
  {
    if (_callbacks[SLIDER_INITIAL_TOUCH] != (fSliderCallback)NULL)
    {
      _callbacks[SLIDER_INITIAL_TOUCH](this, SLIDER_INITIAL_TOUCH, slider_struct->StaticStatus.Position);
    }
    onInitialTouch(slider_struct->StaticStatus.Position);
  }
  else if (slider_struct->StaticStatus.Touch == 0u)
  {
    if (_callbacks[SLIDER_ALL_RELEASE] != (fSliderCallback)NULL)
    {
      _callbacks[SLIDER_ALL_RELEASE](this, SLIDER_ALL_RELEASE, slider_struct->StaticStatus.Position);
    }
    onAllRelease(slider_struct->StaticStatus.Position);
  }
}

bool TSSSlider::enable(TSSControlFlag flag)
{
  if (flag & (TSS_CONTROL_EN_SHIFT | TSS_POSITION_DEC_SHIFT))
  {
    uint8_t enable_flag;
    uint16_t config_register;
    uint8_t  rotary_event;
    TSS_CSSlider const *slider_struct = getControlStruct();

    enable_flag = (uint8_t)(flag & 0x1F);
    (enable_flag > 4) ? rotary_event = 1 : rotary_event = 4;
    config_register = TSS_GetSliderConfig(slider_struct->ControlId, rotary_event);
    config_register |= (uint16_t)(1 << enable_flag);
    if (TSS_SetSliderConfig(slider_struct->ControlId, rotary_event, config_register) == TSS_STATUS_OK)
    {
      return true;
    }
  }
  return false;
}

bool TSSSlider::disable(TSSControlFlag flag)
{
  if (flag & (TSS_CONTROL_EN_SHIFT | TSS_POSITION_DEC_SHIFT))
  {
    uint8_t disable_flag;
    uint16_t config_register;
    uint8_t  rotary_event;
    TSS_CSSlider const *slider_struct = getControlStruct();

    disable_flag = (uint16_t)(flag & 0x1F);
    (disable_flag > 4) ? rotary_event = 1 : rotary_event = 4; /* control config/event config */
    config_register = TSS_GetSliderConfig(slider_struct->ControlId, rotary_event);
    config_register &= (uint16_t)~(1u << disable_flag);
    if (TSS_SetSliderConfig(slider_struct->ControlId, rotary_event, config_register) == TSS_STATUS_OK)
    {
      return true;
    }
  }
  return false;
}

bool TSSSlider::set(TSSControlConfig config, uint8_t value)
{
  if (!(config & (TSSControlConfig)TSS_READ_ONLY_SHIFT) && (config & (TSS_CONTROL_SET_SHIFT | TSS_POSITION_DEC_SET_SHIFT)))
  {
    uint8_t set_flag;
    TSS_CSSlider const *slider_struct = getControlStruct();

    set_flag = (uint8_t)(config & 0x1F);
    if (TSS_SetSliderConfig(slider_struct->ControlId, set_flag , value) == TSS_STATUS_OK)
    {
      return true;
    }
  }
  return false;
}

/* Internal get event function */
static uint16_t slider_get_internal(uint16_t event_flag, TSS_CSSlider const *slider_struct)
{
  uint16_t get_flag = TSS_ERROR_GETCONF_ILLEGAL_PARAMETER;

  if (event_flag & TSS_READ_ONLY_SHIFT)
  {
    event_flag = (uint16_t)(event_flag & 0x1Fu);

    switch(event_flag)
    {
      case 0:
        get_flag = slider_struct->StaticStatus.Position;
        break;
      case 1:
        get_flag = slider_struct->DynamicStatus.Movement;
        break;
      case 2:
        get_flag = slider_struct->DynamicStatus.Direction;
        break;
      case 3:
        get_flag = slider_struct->DynamicStatus.Displacement;
        break;
      case 4:
        get_flag = slider_struct->StaticStatus.Touch;
        break;
      case 5:
        get_flag = slider_struct->StaticStatus.InvalidPos;
        break;
    }
  }
  else
  {
    event_flag = (uint16_t)(event_flag & 0x1Fu);
    get_flag = TSS_GetSliderConfig(slider_struct->ControlId, (uint8_t)event_flag);
  }

  return  get_flag;
}

UINT16 TSSSlider::get(TSSControlConfig config) const
{
  if (config & (TSS_CONTROL_SET_SHIFT | TSS_POSITION_DEC_SET_SHIFT))
  {
    TSS_CSSlider const *slider_struct = getControlStruct();

    return slider_get_internal(config, slider_struct);
  }
  return TSS_ERROR_GETCONF_ILLEGAL_PARAMETER;
}

void TSSSlider::onInitialTouch(uint8_t position)
{

}

void TSSSlider::onAllRelease(uint8_t position)
{

}

void TSSSlider::onMovement(uint8_t position)
{

}

}  /* end of tss namespace */

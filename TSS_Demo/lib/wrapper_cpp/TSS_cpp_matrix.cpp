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
 *   This file contains TSSMatrix control implementation.
 *
 *END************************************************************************/
 #include "TSS_cpp.h"

namespace tss {

TSSMatrix::TSSMatrix(UINT8 control_num)
  : _callbacks()
{
  TSS_SetControlPrivateData(control_num, static_cast<void *>(this));
  setControlStruct(static_cast<TSS_CSMatrix*>(TSS_GetControlStruct(control_num)));

  /* Explicit call, no virtual inside ctor */
  TSSMatrix::enable(TSS_MOVEMENT);
  TSSMatrix::enable(TSS_CONTROL);
  TSSMatrix::enable(TSS_CALLBACK);
}

TSSMatrix::~TSSMatrix()
{
  TSS_CSMatrix const *matrix_struct = static_cast<TSS_CSMatrix const*>(getControlStruct());

  TSS_SetControlPrivateData(matrix_struct->ControlId.ControlNumber, NULL);
  TSSMatrix::disable(TSS_CONTROL);
}

void TSSMatrix::callbackControl(void)
{
  TSS_CSMatrix *matrix_struct = (TSS_CSMatrix *)getControlStruct();

  if ((matrix_struct->DynamicStatusX.Movement == 1) || (matrix_struct->DynamicStatusY.Movement == 1))
  {
    if (_callbacks[MATRIX_MOVEMENT] != (fMatrixCallback)NULL)
    {
      _callbacks[MATRIX_MOVEMENT](this, MATRIX_MOVEMENT, matrix_struct->PositionX, matrix_struct->PositionY);
    }
  }
  else if (matrix_struct->Events.Touch != 0u)
  {
    if (_callbacks[MATRIX_INITIAL_TOUCH] != (fMatrixCallback)NULL)
    {
      _callbacks[MATRIX_INITIAL_TOUCH](this, MATRIX_INITIAL_TOUCH, matrix_struct->PositionX, matrix_struct->PositionY);
    }
  }
  else if (matrix_struct->Events.Touch == 0u)
  {
    if (_callbacks[MATRIX_ALL_RELEASE] != (fMatrixCallback)NULL)
    {
      _callbacks[MATRIX_ALL_RELEASE](this, MATRIX_ALL_RELEASE, matrix_struct->PositionX, matrix_struct->PositionY);
    }
  }
  else if (matrix_struct->Events.Gesture != 0u)
  {
    if (_callbacks[MATRIX_GESTURES] != (fMatrixCallback)NULL)
    {
      _callbacks[MATRIX_GESTURES](this, MATRIX_GESTURES, matrix_struct->GestureDistanceX, matrix_struct->GestureDistanceY);
    }
  }
}

bool TSSMatrix::enable(TSSControlFlag flag)
{
  if (flag & (TSS_CONTROL_EN_SHIFT | TSS_POSITION_DEC_SHIFT | TSS_MATRIX_SHIFT))
  {
    uint8_t enable_flag;
    uint16_t config_register;
    uint8_t  matrix_event;
    TSS_CSMatrix const *matrix_struct = static_cast<TSS_CSMatrix const*>(getControlStruct());

    enable_flag = (uint8_t)(flag & 0x1Fu);
    (enable_flag > 5u) ? matrix_event = 1u : matrix_event = 2u;
    config_register = TSS_GetMatrixConfig(matrix_struct->ControlId, matrix_event);
    config_register |= (uint16_t)(1 << enable_flag);
    if (TSS_SetMatrixConfig(matrix_struct->ControlId, matrix_event, config_register) == TSS_STATUS_OK)
    {
      return true;
    }
  }
  return false;
}

bool TSSMatrix::disable(TSSControlFlag flag)
{
  if (flag & (TSS_CONTROL_EN_SHIFT | TSS_POSITION_DEC_SHIFT | TSS_MATRIX_SHIFT))
  {
    uint8_t disable_flag;
    uint16_t config_register;
    uint8_t  matrix_event;
    TSS_CSMatrix const *matrix_struct = static_cast<TSS_CSMatrix const*>(getControlStruct());

    disable_flag = (uint16_t)(flag & 0x1F);
    (disable_flag > 5u) ? matrix_event = 1u : matrix_event = 2u; /* control config/event config */
    config_register = TSS_GetMatrixConfig(matrix_struct->ControlId, matrix_event);
    config_register &= (uint16_t)~(1u << disable_flag);
    if (TSS_SetMatrixConfig(matrix_struct->ControlId, matrix_event, config_register) == TSS_STATUS_OK)
    {
      return true;
    }
  }
  return false;
}

static uint8_t matrix_get_set_event(uint16_t event_flag)
{
  if (event_flag == TSS_MOVEMENT_TIMEOUT_RW)
  {
    return Matrix_MovementTimeout_Register;
  }
  else if (event_flag == TSS_AUTOREPEAT_RATE_RW)
  {
    return Matrix_AutoRepeatRate_Register;
  }
  else
  {
    return (uint8_t)(event_flag & 0x1F);
  }
}

bool TSSMatrix::set(TSSControlConfig config, uint8_t value)
{
  /* only RW flags can be set */
  if (!(config & (TSSControlConfig)TSS_READ_ONLY_SHIFT) && (config & (TSS_CONTROL_SET_SHIFT | TSS_POSITION_DEC_SET_SHIFT | TSS_MATRIX_SET_SHIFT)))
  {
    uint8_t set_flag;
    TSS_CSMatrix const *matrix_struct = static_cast<TSS_CSMatrix const*>(getControlStruct());

    set_flag = matrix_get_set_event(config);
    if (TSS_SetMatrixConfig(matrix_struct->ControlId, set_flag , value) ==TSS_STATUS_OK)
    {
      return true;
    }
  }
  return false;
}

/* Internal get event function */
static uint16_t matrix_get_internal(uint16_t event_flag, TSS_CSMatrix const *matrix_struct)
{
  uint16_t get_flag = TSS_ERROR_GETCONF_ILLEGAL_PARAMETER;

  if (event_flag & TSS_READ_ONLY_SHIFT)
  {
    event_flag = (uint16_t)(event_flag & 0x1Fu);

    switch(event_flag)
    {
      case 0:
      case 1:
      case 2:
      case 3:
        get_flag = TSS_ERROR_GETCONF_ILLEGAL_PARAMETER;
        break;
      case 4:
        get_flag = matrix_struct->Events.Touch;
        break;
      case 5:
        get_flag = TSS_ERROR_GETCONF_ILLEGAL_PARAMETER;
        break;
      case 6:
        get_flag = matrix_struct->PositionX;
        break;
      case 7:
        get_flag = matrix_struct->PositionY;
        break;
      case 8:
        get_flag = matrix_struct->GestureDistanceX;
        break;
      case 9:
        get_flag = matrix_struct->GestureDistanceY;
        break;
      case 10:
        get_flag = matrix_struct->Events.Gesture;
        break;
    }
  }
  else
  {
    event_flag = (uint16_t)(event_flag & 0x1Fu);
    switch(event_flag)
    {
      case 5:
        event_flag = 3;
      case 6:
        event_flag = 4;
    }
    get_flag = TSS_GetMatrixConfig(matrix_struct->ControlId, (uint8_t)event_flag);
  }

  return  get_flag;
}

uint16_t TSSMatrix::get(TSSControlConfig config) const
{
  if (config & (TSS_CONTROL_SET_SHIFT | TSS_POSITION_DEC_SET_SHIFT | TSS_MATRIX_SET_SHIFT))
  {
    TSS_CSMatrix const *matrix_struct = static_cast<TSS_CSMatrix const*>(getControlStruct());

    return matrix_get_internal(config, matrix_struct);
  }
  return TSS_ERROR_GETCONF_ILLEGAL_PARAMETER;
}

void TSSMatrix::onInitialTouch(uint8_t position_x, uint8_t position_y)
{

}

void TSSMatrix::onAllRelease(uint8_t position_x, uint8_t position_y)
{

}

void TSSMatrix::onMovement(uint8_t position_x, uint8_t position_y)
{

}

void TSSMatrix::onGestures(uint8_t ges_distance_x, uint8_t ges_distance_y)
{

}


} /* end of tss namespace */

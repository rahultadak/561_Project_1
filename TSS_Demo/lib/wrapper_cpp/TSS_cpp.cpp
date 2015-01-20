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
 *   TSS System and Control factory implementation, general C callbacks.
 *
 *END************************************************************************/
#include "TSS_cpp.h"

namespace tss {

TSSControlFactory::TSSControlFactory()
{
  /* Empty ctor */
}

TSSControlFactory::~TSSControlFactory()
{
  /* Empty dtor */
}

TSSControl* TSSControlFactory::createTSSControl(uint8_t control_number)
{
  void *control = TSS_GetControlPrivateData(control_number); /* retrieve this pointer as void* */

  if (control != NULL)
  {
    return static_cast<TSSControl*>NULL;  /* control was already created */
  }

  switch (control_number)
  {
    #if (TSS_N_CONTROLS > 0)
      case 0:
        return new TSS_C0_CLASS(control_number);
    #endif
    #if (TSS_N_CONTROLS > 1)
      case 1:
        return new TSS_C1_CLASS(control_number);
    #endif
    #if (TSS_N_CONTROLS > 2)
      case 2:
        return new TSS_C2_CLASS(control_number);
    #endif
    #if (TSS_N_CONTROLS > 3)
      case 3:
        return new TSS_C3_CLASS(control_number);
    #endif
    #if (TSS_N_CONTROLS > 4)
      case 4:
        return new TSS_C4_CLASS(control_number);
    #endif
    #if (TSS_N_CONTROLS > 5)
      case 5:
        return new TSS_C5_CLASS(control_number);
    #endif
    #if (TSS_N_CONTROLS > 6)
      case 6:
        return new TSS_C6_CLASS(control_number);
    #endif
    #if (TSS_N_CONTROLS > 7)
      case 7:
        return new TSS_C7_CLASS(control_number);
    #endif
    #if (TSS_N_CONTROLS > 8)
      case 8:
        return new TSS_C8_CLASS(control_number);
    #endif
    #if (TSS_N_CONTROLS > 9)
      case 9:
        return new TSS_C9_CLASS(control_number);
    #endif
    #if (TSS_N_CONTROLS > 10)
      case 10:
        return new TSS_C10_CLASS(control_number);
    #endif
    #if (TSS_N_CONTROLS > 11)
      case 11:
        return new TSS_C11_CLASS(control_number);
    #endif
    #if (TSS_N_CONTROLS > 12)
      case 12:
        return new TSS_C12_CLASS(control_number);
    #endif
    #if (TSS_N_CONTROLS > 13)
      case 13:
        return new TSS_C13_CLASS(control_number);
    #endif
    #if (TSS_N_CONTROLS > 14)
      case 14:
        return new TSS_C14_CLASS(control_number);
    #endif
    #if (TSS_N_CONTROLS > 15)
      case 15:
        return new TSS_C15_CLASS(control_number);
    #endif
  }

  return static_cast<TSSControl*>NULL;
}

TSSControl* TSSControlFactory::getTSSControl(uint8_t control_number)
{
  return static_cast<TSSControl*>(TSS_GetControlPrivateData(control_number));
}

TSSSystem::TSSSystem()
{
  if (TSS_Init() != TSS_STATUS_OK)
  {
    callbackSysFaults(SYSTEM_FAULT, SYSTEM_INITIALIZATION_FAULT);
    return;
  }

  #if (TSS_USE_AUTO_SENS_CALIBRATION == 0)
    for (uint8_t i = 0u; i < TSS_N_ELECTRODES; i++)
    {
      setSensitivity(i, 0x1A);  /* Sensitivity set if AUTOCALIB is OFF */
    }
  #endif

  set(TSS_TRIGGER, TSS_TRIGGER_MODE_ALWAYS);
  enable(TSS_SYSTEM);
  enable(TSS_DC_TRACK);
}

TSSSystem::~TSSSystem()
{
  /* Empty dtor */
}

TSSSystem* tss::TSSSystem::getTSSSystem()
{
  static TSSSystem tss_instance; /* Singleton TSSSystem */

  return &tss_instance;
}

bool TSSSystem::isElecTouched(uint8_t elec_num) const
{
  if (elec_num < TSS_N_ELECTRODES)
  {
    uint8_t elec_shift = (uint8_t)(elec_num >> 3u);

    uint8_t elec_touched_temp = (uint8_t)TSS_GetSystemConfig(System_ElectrodeStatus_Register + elec_shift);
    return (elec_touched_temp & (uint8_t)(1u << (uint8_t)(elec_num & 0x07u)));
  }
  else
  {
    return false;
  }
}

bool TSSSystem::isElecEnabled(uint8_t elec_num) const
{
  if (elec_num < TSS_N_ELECTRODES)
  {
    uint8_t elec_shift = (uint8_t)(elec_num >> 3u);
    uint8_t elec_enable_temp = (uint8_t)TSS_GetSystemConfig(System_ElectrodeEnablers_Register + elec_shift);

    return (elec_enable_temp & (uint8_t)(1u << (uint8_t)(elec_num & 0x07u)));
  }
  else
  {
    return false;
  }
}

bool TSSSystem::enableElec(uint8_t elec_num)
{
  if (elec_num < TSS_N_ELECTRODES)
  {
    uint8_t elec_shift = (uint8_t)(elec_num >> 3u);
    uint8_t elec_enable_temp = TSS_GetSystemConfig(System_ElectrodeEnablers_Register + elec_shift);

    elec_enable_temp |= (uint8_t)(1u << (uint8_t)(elec_num & 0x07u));
    if (TSS_SetSystemConfig(System_ElectrodeEnablers_Register + elec_shift, elec_enable_temp) == TSS_STATUS_OK)
    {
      return true;
    }
  }
  return false;
}

bool TSSSystem::disableElec(uint8_t elec_num)
{
  bool result = false;

  if (elec_num < TSS_N_ELECTRODES)
  {
    uint8_t elec_shift = (uint8_t)(elec_num >> 3u);
    uint8_t elec_disable_temp = TSS_GetSystemConfig(System_ElectrodeEnablers_Register + elec_shift);

    elec_disable_temp &= ~(1u << (uint8_t)(elec_num & 0x07u));
    if (TSS_SetSystemConfig(System_ElectrodeEnablers_Register + elec_shift, elec_disable_temp) == TSS_STATUS_OK)
    {
      result = true;
    }
  }
  return result;
}

bool TSSSystem::setSensitivity(uint8_t elec_num, uint8_t value)
{
  bool result = false;

  if (elec_num < TSS_N_ELECTRODES)
  {
    if (TSS_SetSystemConfig(System_Sensitivity_Register + elec_num, value) == TSS_STATUS_OK)
    {
      result = true;
    }
  }
  return result;
}

uint8_t TSSSystem::getSensitivity(uint8_t elec_num) const
{
  if (elec_num < TSS_N_ELECTRODES)
  {
    return (uint8_t)TSS_GetSystemConfig(System_Sensitivity_Register + elec_num);
  }
  else
  {
    return 0; /* 0 is not valid elec num */
  }
}

bool TSSSystem::enableDCTracker(uint8_t elec_num)
{
  if (elec_num < TSS_N_ELECTRODES)
  {
    uint8_t elec_shift = (uint8_t)(elec_num >> 3u);
    uint8_t elec_enable_temp = TSS_GetSystemConfig(System_DCTrackerEnablers_Register + elec_shift);

    elec_enable_temp |= (uint8_t)(1u << (uint8_t)(elec_num & 0x07u));
    if (TSS_SetSystemConfig(System_DCTrackerEnablers_Register + elec_shift, elec_enable_temp) == TSS_STATUS_OK)
    {
      return true;
    }
  }
  return false;
}

bool TSSSystem::disableDCTracker(uint8_t elec_num)
{
  if (elec_num < TSS_N_ELECTRODES)
  {
    uint8_t elec_shift = (uint8_t)(elec_num >> 3u);
    uint8_t elec_disable_temp = TSS_GetSystemConfig(System_DCTrackerEnablers_Register + elec_shift);

    elec_disable_temp &= ~(1u << (uint8_t)(elec_num & 0x07u));
    if (TSS_SetSystemConfig(System_DCTrackerEnablers_Register + elec_shift, elec_disable_temp) == TSS_STATUS_OK)
    {
      return true;
    }
  }
  return false;
}

bool TSSSystem::setBaseline(uint8_t elec_num, uint16_t value)
{
  bool result = false;

  if (elec_num < TSS_N_ELECTRODES)
  {
    if (TSS_SetSystemConfig(System_Baseline_Register + elec_num, value) == TSS_STATUS_OK)
    {
      result = true;
    }
  }
  return result;
}

uint16_t TSSSystem::getBaseline(uint8_t elec_num)
{
  if (elec_num < TSS_N_ELECTRODES)
  {
    return TSS_GetSystemConfig(System_Baseline_Register + elec_num);
  }
  else
  {
    return 0; /* 0 is not valid elec num */
  }
}

bool TSSSystem::enable(TSSSystemFlag flag)
{
  bool result = false;
  uint16_t TSS_control_register =  TSS_GetSystemConfig(System_SystemConfig_Register);

  if (TSS_control_register != TSS_ERROR_GETCONF_ILLEGAL_PARAMETER)
  {
    TSS_control_register |= (uint16_t)(1u << flag);
    if (TSS_SetSystemConfig(System_SystemConfig_Register, TSS_control_register) == TSS_STATUS_OK)
    {
      result = true;
    }
  }
  return result;
}

bool TSSSystem::disable(TSSSystemFlag flag)
{
  uint16_t TSS_control_register =  TSS_GetSystemConfig(System_SystemConfig_Register);

  if (TSS_control_register != TSS_ERROR_GETCONF_ILLEGAL_PARAMETER)
  {
    TSS_control_register &= (uint16_t)(~(uint16_t)(1u << flag));
    TSS_SetSystemConfig(System_SystemConfig_Register, TSS_control_register);
    return true;
  }
  else
  {
    return false;
  }
}

bool TSSSystem::set(TSSSystemConfig config, uint16_t value)
{
  if (TSS_SetSystemConfig(config, value) == TSS_STATUS_OK)
  {
    return true;
  }
  else
  {
    return false;
  }
}

void TSSSystem::callbackSysFaults(TSSSystemEvent event, uint8_t elec_num)
{
  if (_callbacks[FAULT] != (TSS_fSystemCallback)NULL)
  {
    _callbacks[FAULT](this, event, elec_num);
  }
  onFault(event, elec_num);
}

void TSSSystem::onFault(TSSSystemEvent event, uint8_t elec_num)
{

}

TSSControl::~TSSControl()
{
  /* Empty dtor */
}

} /* end of tss namespace */

void callbackFaultHelper(uint8_t elec_num)
{
  tss::TSSSystem *system = tss::TSSSystem::getTSSSystem();
  tss::TSSSystemEvent fault = tss::FAULT;

  if (tss_CSSys.Faults.ChargeTimeout)
  {
    fault = tss::CHARGE_TIMEOUT_FAULT;
  }
  else if (tss_CSSys.Faults.SmallCapacitor)
  {
    fault = tss::SMALL_CAPACITOR_FAULT;
  }
  else if (tss_CSSys.Faults.SmallTriggerPeriod)
  {
    fault = tss::SMALL_TRIGGER_FAULT;
  }
  else if (tss_CSSys.Faults.DataCorruption)
  {
    fault = tss::DATA_CORRUPTION_FAULT;
  }

  system->callbackSysFaults(fault, elec_num);
}

void callbackControlHelper(TSS_CONTROL_ID control_id)
{
  void *this_pointer = TSS_GetControlPrivateData(control_id.ControlNumber); /* retrieve this pointer as void* */

  if (this_pointer != NULL)
  {
    tss::TSSControl *control = static_cast<tss::TSSControl *>(this_pointer);
    control->callbackControl();
  }
}



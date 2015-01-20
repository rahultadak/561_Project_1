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
 *   The C++ wrapper header file.
 *
 *END************************************************************************/
#ifndef TSS_CPP_H
#define TSS_CPP_H

#include "TSS_DataTypes.h"
#include "TSS_API.h"
#include "TSS_cpp_const.h"

namespace tss {

/** Forward declaration for typedefs
 */
class TSSControl;
class TSSSystem;

/** Callbacks types
*/
typedef void (* TSS_fControlCallback)(TSSControl *, uint8_t, uint8_t);
typedef void (* TSS_fSystemCallback)(TSSSystem *, TSSSystemEvent, uint8_t);

/** Code generation for callbacks.
 */
#define IMPLEMENT_CALLBACKS(num, fptr, fptr_data)           \
  public:                                                   \
    virtual uint8_t getCallbacks(fptr **callback)           \
    {                                                       \
      *callback = (fptr*)(&_callbacks[0]);                  \
      return num;                                           \
    }                                                       \
  private:                                                  \
    fptr_data _callbacks[num]

/** Declares a copy ctor and an assignment ctor. These are
 *  placed in a private section which prohibits them to be invoked.
 *  No definition provided.
 */
#define PROHIBIT_COPY_AND_ASSIGN(className) \
  className(const className&);              \
  className& operator=(const className&)

/** TSS Callback class allows to register and to unregister callbacks.
 *  @tparam fUserCallback The type of callbacks.
 *  @tparam E Events enumeration for the inherited class.
 */
template<typename fUserCallback, typename E>
class TSSCallback
{
  public:
    /** Get a pointer to callback's array.
     *  @tparam callback An address of callbacks.
     *  @returns
     *    A number of callbacks available.
     */
    virtual uint8_t getCallbacks(fUserCallback **callback) = 0;

    /** Callback registration.
     */
    bool regCallback(fUserCallback callback, E state = static_cast<E>(0));

    /** Callback unregistration.
     */
    bool unregCallback(E state = static_cast<E>(0));
};

/** Callback registration
 *  @tparam fUserCallback The type of callbacks.
 *  @tparam E Events enumeration for the inherited class.
 */
template<typename fUserCallback, typename E>
bool TSSCallback<fUserCallback, E>::regCallback(fUserCallback user_callback, E state)
{
  bool result = false;
  fUserCallback *control_callbacks;
  uint8_t num_of_callbacks = getCallbacks(&control_callbacks);

  if (state < num_of_callbacks)
  {
    control_callbacks[state] = user_callback;
    result = true;
  }
  return result;
}

/** Callback unregistration
 *  @tparam fUserCallback The type of callbacks.
 *  @tparam E Events enumeration for the inherited class.
 */
template<typename fUserCallback, typename E>
bool TSSCallback<fUserCallback, E>::unregCallback(E state)
{
  return regCallback(0, state); /* NULL as a parameter */
}

/** TSS Control Factory class allows to create TSS Control. The parameter
 *  reflects the control number defined in the TSSSystemSetup header file.
 */
class TSSControlFactory
{
  public:
    /** Empty ctor
     */
    TSSControlFactory();

    /* Empty dtor
     */
    ~TSSControlFactory();

    /** Creates new control object and returns the pointer to the allocated object.
     *  @param  control_number  number of a control according to the TSSSystemSetup header file.
     *  @returns
     *    TSSControl* valid (not NULL) pointer to the control.
     *    NULL failed.
     */
    static TSSControl* createTSSControl(uint8_t control_number = 0);

    /** Returns created control if it has been already instantiated.
     *  @returns
     *    valid pointer to the control.
     *    NULL control was not instantiated yet or invalid control number.
     */
    static TSSControl* getTSSControl(uint8_t control_number);

  private:
    /** A copy and an assigment ctor.
     */
    PROHIBIT_COPY_AND_ASSIGN(TSSControlFactory);
};

/** TSS System class (singleton) provides the interface for TSS.
 */
class TSSSystem : public TSSCallback<TSS_fSystemCallback, TSSSystemEvent>
{
  public:
    /** Returns pointer to the TSSSystem object.
     *  @returns
     *    Pointer to the TSSSystem.
     */
    static TSSSystem* getTSSSystem(void);

    /** TSS_Task function. This function should be periodically called to
     *  provide CPU time to TSS.
     */
    static uint8_t TSSTask(void)
    {
      return TSS_Task();
    }

    /** Determine if an electrode is touched or released.
     *  @param elec_num The electrode number.
     *  @returns
     *    true if the electrode is touched,
     *    false otherwise.
     */
    bool isElecTouched(uint8_t elec_num) const;

    /** Determine if an electrode is enabled or disabled.
     *  @param elec_num The electrode number
     *  @returns
     *    true if the electrode is enabled,
     *    false otherwise.
     */
    bool isElecEnabled(uint8_t elec_num) const;

    /** Enable an electrode.
     * @param elec_num The electrode number.
     * @returns
     *   true if the electrode was enabled,
     *   false otherwise.
     */
    bool enableElec(uint8_t elec_num);

    /** Disable an electrode.
     *  @param elec_num The electrode number.
     *  @returns
     *    true if the electrode was disabled,
     *    false otherwise.
     */
    bool disableElec(uint8_t elec_num);

    /** Set a sensitivity value for specified electrode.
     *  @param elec_num The electrode number.
     *  @param value  Sensitivity value to be set.
     *  @returns
     *    true if the sensitivity was set.
     *    false otherwise.
     */
    bool setSensitivity(uint8_t elec_num, uint8_t value);

    /** Return a sensitivity value for specified electrode.
     *  @param elec_num The electrode number.
     *  @returns
     *    sensitivity value
     *    error 0                                   - non-valid sensitivity value
     *          TSS_ERROR_GETCONF_ILLEGAL_PARAMETER - wrong parameter
     */
    uint8_t getSensitivity(uint8_t elec_num) const;

    /** Enable DC Tracker for specified electrode.
     *  @param elec_num The electrode number
     *  @returns
     *    true if electrode was enabled
     *    false otherwise
     */
    bool enableDCTracker(uint8_t elec_num);

    /** Disable DC Tracker for specified electrode.
     *  @param elec_num The electrode number
     *  @returns
     *    true if electrode was disabled
     *    false otherwise
     */
    bool disableDCTracker(uint8_t elec_num);

    /** Set baseline for specified electrode.
     *  @param elec_num The electrode number
     *  @param value Baseline value to be set
     *  @returns
     *    true if electrode was set
     *    false otherwise
     */
    bool setBaseline(uint8_t elec_num, uint16_t value);

    /** Get baseline for specified electrode.
     *  @param elec_num The electrode number
     *  @returns
     *    baseline value
     *    TSS_ERROR_GETCONF_ILLEGAL_PARAMETER - wrong parameter
     */
    uint16_t getBaseline(uint8_t elec_num);

    /** Enable TSS System features. Flags are defined in the TSS_cpp_const header file.
     *  @param flag Any of TSS System's flags.
     *  @returns
     *   true  if flag was enabled,
     *   false otherwise.
     */
    bool enable(TSSSystemFlag flag);

    /** Disable TSS System feature. Flags are defined in the TSS_cpp_const header file.
     *  @param flag Any of TSS System's flags.
     *  @returns
     *    true if the flag was disabled,
     *    false otherwise.
     */
    bool disable(TSSSystemFlag flag);

    /** Set TSS System value.
     *  @param command TSS System's config flag to be set.
     *  @param value Value to be set
     *  @returns
     *    true if the value was set,
     *    false otherwise.
     */
    bool set(TSSSystemConfig command, uint16_t value);

    /** Get TSS System config value.
     *  @param command TSS System's config flag to be retrieved.
     *  @returns
     *    Requested configuration value.
     */
    uint16_t get(TSSSystemConfig command) const
    {
      return TSS_GetSystemConfig(command);
    }

    /** This callback calls the user's callback and onFault callback in case
     *  a fault has occurred. See TSS_cpp_const header file for available
     *  faults flags.
     *  @param elec_num Electrode number which causes a fault (not valid for system faults).
     *  @param event System fault which has occurred.
     */
    void callbackSysFaults(TSSSystemEvent event, uint8_t elec_num);

    /** System's internal virtual on Fault callback.
     *  @param event Flag which has occurred
     *  @param elec_num Electrode number which causes a fault (not valid for system faults).
     */
    virtual void onFault(TSSSystemEvent event, uint8_t elec_num);

    /** Macro for callback's implementation. The TSS System provides one callback:
     *  FAULT (callback parameters - this, fault, electrode number (if any)).
     */
    IMPLEMENT_CALLBACKS(1, TSS_fSystemCallback, TSS_fSystemCallback);

  private:
    /** Initialize TSS and all electrodes.
     */
    TSSSystem();

    /** Empty dtor.
     */
    ~TSSSystem();

    /** A copy and an assigment ctor.
     */
    PROHIBIT_COPY_AND_ASSIGN(TSSSystem);
};

/** TSS Control interface class which defines an interface for TSS controls.
 *  TSS Control does not provide registration/unregistration of callbacks
 *  because they are specific for each type of a control.
 */
class TSSControl : protected TSSCallback<TSS_fControlCallback, uint8_t>
{
  public:
    /** Instatiates TSSSystem if it has not been yet.
     */
    TSSControl() : _control_struct(0)
    {
      TSSSystem::getTSSSystem(); /* instantiate TSSSystem */
    }

    /** Empty dtor
     */
    virtual ~TSSControl();

    /** This is a specific callback handler for a control.
     */
    virtual void callbackControl(void) = 0;

    /** Enable specific flags available for CONTROL.
     *  Each control has own specific flags, please refer to TSSControlFlag enum
     *  Common flags for all controls are TSS_CONTROL and TSS_CALLBACK. The rest
     *  of the flags depend on the type of a control.
     *  @param flag Flag to be enabled.
     *  @returns
     *    true if flag was enabled,
     *    false otherwise.
     */
    virtual bool enable(TSSControlFlag flag) = 0;

    /** Disable specific flags available for CONTROL.
     *  Each control has own specific flags, please refer to the TSSControlFlag enum.
     *  @param flag Flag to be disabled.
     *  @returns
     *    true if flag was disabled,
     *    false otherwise.
     */
    virtual bool disable(TSSControlFlag flag) = 0;

    /** Set specific configuration available for CONTROL.
     *  Each control has own specific flags, please refer to the TSSControlConfig enum.
     *  @param config Command to be set.
     *  @param value Value to be set.
     *  @returns
     *    true config was set,
     *    false otherwise.
     */
    virtual bool set(TSSControlConfig config, uint8_t value) = 0;

    /** Get specific configuration available for CONTROL.
     *  Each control has own specific flags, please refer to the TSSControlConfig enum.
     *  @param config Command to be set.
     *  @returns
     *    config value,
     *    TSS_ERROR_GETCONF_ILLEGAL_CONTROL_TYPE otherwise.
     */
    virtual uint16_t get(TSSControlConfig config) const = 0;

    /** Returns the pointer to the control's structure.
     */
    const void* getControlStructInternal(void) const
    {
      return _control_struct;
    }

    /** Set the member pointer to the control's structure.
     */
    void setControlStructInternal(void const *control_struct)
    {
      _control_struct = control_struct;
    }

private:
    /** Pointer to the TSS control struct.
     */
    void const *_control_struct;
};

/** TSS Keypad control
*/
class TSSKeypad : public TSSControl
{
  public:
    /** Keypad callback events
    */
    enum KeypadEvent
    {
      KEYPAD_RELEASE = 0,
      KEYPAD_TOUCH = 1,
      KEYPAD_KEYS_EXCEEDED = 2
    };

    /** Keypad's callbacks typedef
     */
    typedef void (* fKeypadCallback)(TSSKeypad *, KeypadEvent, uint8_t);

    /** Initialize KEYPAD control according to the TSSSystemSetup header file.
     *  @param control_num Control number (for example TSS_C0_TYPE is a control number 0).
     */
    TSSKeypad(uint8_t control_num);

    /** Disables a control in TSS.
     */
    virtual ~TSSKeypad();

    /** This is the callback handler for the keypad control.
     *  It reads the internal keypad buffer and invokes an action which is available
     *  via callbacks or the public method like onTouch, etc.
     */
    virtual void callbackControl(void);

    /** Enable specific flags available for the keypad control.
     *  Refer to the TSSControlFlag enum.
     *  @param flag Flag to be enabled.
     *  @returns
     *    true if flag was enabled,
     *    false otherwise.
     */
    virtual bool enable(TSSControlFlag flag);

    /** Disable specific flags available for the keypad control.
     *  Refer to the TSSControlFlag enum.
     *  @param flag Flag to be disabled.
     *  @returns
     *    true if flag was disabled,
     *    false otherwise.
     */
    virtual bool disable(TSSControlFlag flag);

    /** Set specific configuration available for the keypad control.
     *  Each control has own specific flags, please refer to the TSSControlConfig enum.
     *  @param command Command to be set.
     *  @param value Value to be set.
     *  @returns
     *    true config was set,
     *    false otherwise.
     */
    virtual bool set(TSSControlConfig command, uint8_t value);

    /** Get specific configuration available for the keypad control.
     *  Each control has own specific flags, please refer to the TSSControlConfig enum.
     *  @param command Command to be set.
     *  @returns
     *    config value,
     *    TSS_ERROR_GETCONF_ILLEGAL_CONTROL_TYPE otherwise.
     */
    virtual uint16_t get(TSSControlConfig command) const;

    /** Returns the TSS control's structure
     */
    const TSS_CSKeypad* getControlStruct(void) const
    {
      return static_cast<TSS_CSKeypad const*>(getControlStructInternal());
    }

    /** Set the member pointer to the control's structure.
     */
    void setControlStruct(TSS_CSKeypad *control_struct)
    {
      setControlStructInternal(static_cast<void const *>(control_struct));
    }

    /** Registers callback. It directs call to the base class with cast.
     *  @param callback Callback to be registered
     *  @param event Event which invokes a callback
     *  @returns
     *    true if successful
     *    false otherwise
     */
    bool regCallback(fKeypadCallback callback, KeypadEvent event)
    {
      /* _cast - Casting for a higher level, it stores the callback, a callback invokation
         is casted back to it's proper type.
      */
      return TSSControl::regCallback((TSS_fControlCallback)callback, event);
    }

    /** Unregisters callback. It directs call to the base class with cast.
     *  @param event Event for which is a callback be disabled
     *  @returns
     *    true if successful
     *    false otherwise
     */
    bool unregCallback(KeypadEvent event)
    {
      return TSSControl::unregCallback(event);
    }

    /** onTouch callback is generated only if the keypad's touch flag is enabled
     *  and a touch has been detected.
     *  @param elec_num The number of the electrode which generated callback
    */
    virtual void onTouch(uint8_t elec_num);

    /** onRelease callback is generated only if the keypad's release enabler
     *  is enabled and a release has been detected.
     *  @param elec_num The number of the electrode which generated callback.
     */
    virtual void onRelease(uint8_t elec_num);

    /** onExceededKeys callback is generated only if the keypad's exceeded keys
     *  enabler is enabled and the number of keys has exceeded the limit.
     *  @param max_touches The limit which was exceeded.
     */
    virtual void onExceededKeys(uint8_t max_touches);

    /** Macro for callback's implementation. The keypad control provides 3 callbacks:
     *  KEYPAD_RELEASE (callback parameters - this, KEYPAD_RELEASE, electrode number).
     *  KEYPAD_TOUCH (callback parameters - this, KEYPAD_TOUCH, electrode number).
     *  KEYPAD_KEYS_EXCEEDED (callback parameters - this, KEYPAD_KEYS_EXCEEDED, limit which was exceeded).
     */
    IMPLEMENT_CALLBACKS(3, TSS_fControlCallback, fKeypadCallback);

  private:
    /** A copy and an assigment ctor.
     */
    PROHIBIT_COPY_AND_ASSIGN(TSSKeypad);
};

/** TSS Analog Slider control
 */
class TSSASlider : public TSSControl
{
  public:
    /** Analog Slider callback events
    */
    enum ASliderEvent
    {
      ASLIDER_ALL_RELEASE = 0,
      ASLIDER_INITIAL_TOUCH = 1,
      ASLIDER_MOVEMENT = 2
    };

    /** Analog Slider's callbacks typedef
     */
    typedef void (* fASliderCallback)(TSSASlider *, ASliderEvent, uint8_t);

    /** Initializes ASLIDER control according to the TSSSystemSetup header file.
     *  @param control_num Control number (for example TSS_C0_TYPE is a control number 0).
     */
    TSSASlider(uint8_t control_num);

    /** Disables a control in TSS.
     */
    virtual ~TSSASlider();

    /** This is the callback handler for the analog slider control.
     *  It invokes an action which is available via callbacks or public method
     *  like onInitialTouch, etc.
     */
    virtual void callbackControl(void);

    /** Enable specific flags available for the analog slider control.
     *  Refer to the TSSControlFlag enum.
     *  @param flag Flag to be enabled.
     *  @returns
     *    true if flag was enabled,
     *    false otherwise.
     */
    virtual bool enable(TSSControlFlag flag);

    /** Disable specific flags available for the analog slider control.
     *  Refer to the TSSControlFlag enum.
     *  @param flag Flag to be disabled.
     *  @returns
     *    true if flag was disabled,
     *    false otherwise.
     */
    virtual bool disable(TSSControlFlag flag);

    /** Set specific configuration available for the analog slider control.
     *  Each control has own specific flags, please refer to the TSSControlConfig enum.
     *  @param command Command to be set.
     *  @param value Value to be set.
     *  @returns
     *    true config was set,
     *    false otherwise.
     */
    virtual bool set(TSSControlConfig command, uint8_t value);

    /** Get specific configuration available for the analog slider control.
     *  Each control has own specific flags, please refer to the TSSControlConfig enum.
     *  @param command Command to be set.
     *  @returns
     *    config value,
     *    TSS_ERROR_GETCONF_ILLEGAL_CONTROL_TYPE otherwise.
     */
    virtual uint16_t get(TSSControlConfig command) const;

    /** Returns the TSS control's structure
     */
    const TSS_CSASlider* getControlStruct(void) const
    {
      return static_cast<TSS_CSASlider const*>(getControlStructInternal());
    }

    /** Set the member pointer to the control's structure.
     */
    void setControlStruct(TSS_CSASlider *control_struct)
    {
      setControlStructInternal(static_cast<void const *>(control_struct));
    }

    /** Registers callback. It directs call to the base class with cast.
     *  @param callback Callback to be registered
     *  @param event Event which invokes a callback
     *  @returns
     *    true if successful
     *    false otherwise
     */
    bool regCallback(fASliderCallback callback, ASliderEvent event)
    {
      /* _cast - Casting for a higher level, it stores the callback, a callback invokation
         is casted back to it's proper type.
      */
      return TSSControl::regCallback((TSS_fControlCallback)callback, event);
    }

    /** Unregisters callback. It directs call to the base class with cast.
     *  @param event Event for which is a callback be disabled
     *  @returns
     *    true if successful
     *    false otherwise
     */
    bool unregCallback(ASliderEvent event)
    {
      return TSSControl::unregCallback(event);
    }

    /** onInitialTouch callback is generated only if the analog slider's initial
     *  touch enabler is enabled and an initial touch has been detected.
     *  @param position Actual position
    */
    virtual void onInitialTouch(uint8_t position);

    /** onAllRelease callback is generated only if the analog slider's release
     *  enabler is enabled and all electrodes have been released.
     *  @param position Actual position
    */
    virtual void onAllRelease(uint8_t position);

    /** onMovement callback is generated only if the analog slider's movement
     *  enabler is enabled and a movement has been detected.
     *  @param position Actual position
    */
    virtual void onMovement(uint8_t position);

    /** Macro for callback's implementation. The analog slider control provides
     *  3 callbacks:
     *  ASLIDER_ALL_RELEASE (callback parameters - this, ASLIDER_ALL_RELEASE, position).
     *  ASLIDER_INITIAL_TOUCH (callback parameters - this, ASLIDER_INITIAL_TOUCH, position).
     *  ASLIDER_MOVEMENT (callback parameters - this, ASLIDER_MOVEMENT, position).
     */
     IMPLEMENT_CALLBACKS(3, TSS_fControlCallback, fASliderCallback);

  private:
    /** A copy and an assigment ctor.
     */
    PROHIBIT_COPY_AND_ASSIGN(TSSASlider);
};

/** TSS Analog rotary control
*/
class TSSARotary : public TSSControl
{
  public:
    /** Analog Rotary callback events
     */
    enum ARotaryEvent
    {
      AROTARY_ALL_RELEASE = 0,
      AROTARY_INITIAL_TOUCH = 1,
      AROTARY_MOVEMENT = 2
    };

    /** Analog Rotary's callbacks typedef
     */
    typedef void (* fARotaryCallback)(TSSARotary *, ARotaryEvent, uint8_t);

    /** Initializes AROTARY control according to the TSSSystemSetup header file.
     *  @param control_num Control number (for example TSS_C0_TYPE is a control number 0).
     */
    TSSARotary(uint8_t control_num);

    /** Disables a control in TSS.
     */
    virtual ~TSSARotary();

    /** This is the callback handler for the analog rotary control.
     *  It invokes an action which is available via callbacks or public method
     *  like onInitialTouch, etc.
     */
    virtual void callbackControl(void);

    /** Enable specific flags available for the analog rotary control.
     *  Refer to the TSSControlFlag enum.
     *  @param flag Flag to be enabled.
     *  @returns
     *    true if flag was enabled,
     *    false otherwise.
     */
    virtual bool enable(TSSControlFlag flag);

    /** Disable specific flags available for the analog rotary control.
     *  Refer to the TSSControlFlag enum.
     *  @param flag Flag to be disabled.
     *  @returns
     *    true if flag was disabled,
     *    false otherwise.
     */
    virtual bool disable(TSSControlFlag flag);

    /** Set specific configuration available for the analog rotary control.
     *  Each control has own specific flags, please refer to the TSSControlConfig enum.
     *  @param command Command to be set.
     *  @param value Value to be set.
     *  @returns
     *    true config was set,
     *    false otherwise.
     */
    virtual bool set(TSSControlConfig command, uint8_t value);

   /** Get specific configuration available for the analog rotary control.
     *  Each control has own specific flags, please refer to the TSSControlConfig enum.
     *  @param command Command to be set.
     *  @returns
     *    config value,
     *    TSS_ERROR_GETCONF_ILLEGAL_CONTROL_TYPE otherwise.
     */
    virtual uint16_t get(TSSControlConfig command) const;

    /** Returns the TSS control's structure
     */
    const TSS_CSARotary* getControlStruct(void) const
    {
      return static_cast<TSS_CSARotary const*>(getControlStructInternal());
    }

    /** Set the member pointer to the control's structure.
     */
    void setControlStruct(TSS_CSARotary *control_struct)
    {
      setControlStructInternal(static_cast<void const *>(control_struct));
    }

    /** Registers callback. It directs call to the base class with cast.
     *  @param callback Callback to be registered
     *  @param event Event which invokes a callback
     *  @returns
     *    true if successful
     *    false otherwise
     */
    bool regCallback(fARotaryCallback callback, ARotaryEvent event)
    {
      /* _cast - Casting for a higher level, it stores the callback, a callback invokation
         is casted back to it's proper type.
      */
      return TSSControl::regCallback((TSS_fControlCallback)callback, event);
    }

    /** Unregisters callback. It directs call to the base class with cast.
     *  @param event Event for which is a callback be disabled
     *  @returns
     *    true if successful
     *    false otherwise
     */
    bool unregCallback(ARotaryEvent event)
    {
      return TSSControl::unregCallback(event);
    }

    /** onInitialTouch callback is generated only if the analog rotary's initial
     *  touch enabler is enabled and an initial touch has been detected.
     *  @param position Actual position
     */
    virtual void onInitialTouch(uint8_t position);

    /** onAllRelease callback is generated only if the analog rotary's release
     *  enabler is enabled and all electrodes have been released.
     *  @param position Actual position
     */
    virtual void onAllRelease(uint8_t position);

    /** onMovement callback is generated only if the analog rotary's movement
     *  enabler is enabled and a movement has been detected.
     *  @param position Actual position
     */
    virtual void onMovement(uint8_t position);

    /** Macro for callback's implementation. The analog rotary control provides
     *  3 callbacks:
     *  AROTARY_ALL_RELEASE (callback parameters - this, AROTARY_ALL_RELEASE, position).
     *  INITIAL_TOUCH (callback parameters - this, INITIAL_TOUCH, position).
     *  AROTARY_MOVEMENT (callback parameters - this, AROTARY_MOVEMENT, position).
     */
    IMPLEMENT_CALLBACKS(3, TSS_fControlCallback, fARotaryCallback);

  private:
    /** A copy and an assigment ctor.
     */
    PROHIBIT_COPY_AND_ASSIGN(TSSARotary);
};

/** TSS Slider control class
*/
class TSSSlider : public TSSControl
{
  public:
    /** TSS Slider callback events
     */
    enum SliderEvent
    {
      SLIDER_ALL_RELEASE = 0,
      SLIDER_INITIAL_TOUCH = 1,
      SLIDER_MOVEMENT = 2
    };

    /** Slider's callbacks typedef
     */
    typedef void (* fSliderCallback)(TSSSlider *, SliderEvent, uint8_t);

    /** Initializes SLIDER control according to the TSSSystemSetup header file.
     *  @param control_num Control number (for example TSS_C0_TYPE is a control number 0).
     */
    TSSSlider(uint8_t control_num);

    /** Disables a control in TSS.
     */
    virtual ~TSSSlider();

    /** This is the callback handler for the slider control.
     *  It invokes an action which is available via callbacks or public method
     *  like onInitialTouch, etc.
     */
    virtual void callbackControl(void);

    /** Enable specific flags available for the slider control.
     *  Refer to the TSSControlFlag enum.
     *  @param flag Flag to be enabled.
     *  @returns
     *    true if flag was enabled,
     *    false otherwise.
     */
    virtual bool enable(TSSControlFlag flag);

    /** Disable specific flags available for the slider control.
     *  Refer to the TSSControlFlag enum.
     *  @param flag Flag to be disabled.
     *  @returns
     *    true if flag was disabled,
     *    false otherwise.
     */
    virtual bool disable(TSSControlFlag flag);

    /** Set specific configuration available for the slider control.
     *  Each control has own specific flags, please refer to the TSSControlConfig enum.
     *  @param command Command to be set.
     *  @param value Value to be set.
     *  @returns
     *    true config was set,
     *    false otherwise.
     */
    virtual bool set(TSSControlConfig command, uint8_t value);

    /** Get specific configuration available for the slider control.
     *  Each control has own specific flags, please refer to the TSSControlConfig enum.
     *  @param command Command to be set.
     *  @returns
     *    config value,
     *    TSS_ERROR_GETCONF_ILLEGAL_CONTROL_TYPE otherwise.
     */
    virtual uint16_t get(TSSControlConfig command) const;

    /** Returns the TSS control's structure
     */
    const TSS_CSSlider* getControlStruct(void) const
    {
      return static_cast<TSS_CSSlider const*>(getControlStructInternal());
    }

    /** Set the member pointer to the control's structure.
     */
    void setControlStruct(TSS_CSSlider *control_struct)
    {
      setControlStructInternal(static_cast<void const *>(control_struct));
    }

    /** Registers callback. It directs call to the base class with cast.
     *  @param callback Callback to be registered
     *  @param event Event which invokes a callback
     *  @returns
     *    true if successful
     *    false otherwise
     */
    bool regCallback(fSliderCallback callback, SliderEvent event)
    {
      /* _cast - Casting for a higher level, it stores the callback, a callback invokation
         is casted back to it's proper type.
      */
      return TSSControl::regCallback((TSS_fControlCallback)callback, event);
    }

    /** Unregisters callback. It directs call to the base class with cast.
     *  @param event Event for which is a callback be disabled
     *  @returns
     *    true if successful
     *    false otherwise
     */
    bool unregCallback(SliderEvent event)
    {
      return TSSControl::unregCallback(event);
    }

    /** onInitialTouch callback is generated only if the slider's initial
     *  touch enabler is enabled and an initial touch has been detected.
     *  @param position Actual position
    */
    virtual void onInitialTouch(uint8_t position);

    /** onAllRelease callback is generated only if the slider's release
     *  enabler is enabled and all electrodes have been released.
     *  @param position Actual position
    */
    virtual void onAllRelease(uint8_t position);

    /** onMovement callback is generated only if the slider's movement
     *  enabler is enabled and a movement has been detected.
     *  @param position Actual position
    */
    virtual void onMovement(uint8_t position);

    /** Macro for callback's implementation. The slider control provides
     *  3 callbacks:
     *  SLIDER_ALL_RELEASE (callback parameters - this, SLIDER_ALL_RELEASE, position).
     *  SLIDER_INITIAL_TOUCH (callback parameters - this, SLIDER_INITIAL_TOUCH, position).
     *  SLIDER_MOVEMENT (callback parameters - this, SLIDER_MOVEMENT, position).
     */
    IMPLEMENT_CALLBACKS(3, TSS_fControlCallback, fSliderCallback);

  private:
    /** A copy and an assigment ctor.
     */
    PROHIBIT_COPY_AND_ASSIGN(TSSSlider);
};

/** TSS Rotary control class
*/
class TSSRotary : public TSSControl
{
  public:
    /** TSS Rotary callback events
     */
    enum RotaryEvent
    {
      ROTARY_ALL_RELEASE = 0,
      ROTARY_INITIAL_TOUCH = 1,
      ROTARY_MOVEMENT = 2
    };

    /** Rotary's callbacks typedef
     */
    typedef void (* fRotaryCallback)(TSSRotary *, RotaryEvent, uint8_t);

    /** Initializes ROTARY control according to the TSSSystemSetup header file.
     *  @param control_num Control number (for example TSS_C0_TYPE is a control number 0).
     */
    TSSRotary(uint8_t control_num);

    /** Disables a control in TSS.
     */
    virtual ~TSSRotary();

    /** This is the callback handler for the rotary control.
     *  It invokes an action which is available via callbacks or public method
     *  like onInitialTouch, etc.
     */
    virtual void callbackControl(void);

    /** Enable specific flags available for the rotary control.
     *  Refer to the TSSControlFlag enum.
     *  @param flag Flag to be enabled.
     *  @returns
     *    true if flag was enabled,
     *    false otherwise.
     */
    virtual bool enable(TSSControlFlag flag);

   /** Disable specific flags available for the rotary control.
     *  Refer to the TSSControlFlag enum.
     *  @param flag Flag to be disabled.
     *  @returns
     *    true if flag was disabled,
     *    false otherwise.
     */
    virtual bool disable(TSSControlFlag flag);

    /** Set specific configuration available for the rotary control.
     *  Each control has own specific flags, please refer to the TSSControlConfig enum.
     *  @param command Command to be set.
     *  @param value Value to be set.
     *  @returns
     *    true config was set,
     *    false otherwise.
     */
    virtual bool set(TSSControlConfig command, uint8_t value);

    /** Get specific configuration available for the rotary control.
     *  Each control has own specific flags, please refer to the TSSControlConfig enum.
     *  @param command Command to be set.
     *  @returns
     *    config value,
     *    TSS_ERROR_GETCONF_ILLEGAL_CONTROL_TYPE otherwise.
     */
    virtual uint16_t get(TSSControlConfig command) const;

    /** Returns the TSS control's structure
     */
    const TSS_CSRotary* getControlStruct(void) const
    {
      return static_cast<TSS_CSRotary const*>(getControlStructInternal());
    }

    /** Set a pointer to the control's structure.
     */
    void setControlStruct(TSS_CSRotary *control_struct)
    {
      setControlStructInternal(static_cast<void const *>(control_struct));
    }

    /** Registers callback. It directs call to the base class with cast.
     *  @param callback Callback to be registered
     *  @param event Event which invokes a callback
     *  @returns
     *    true if successful
     *    false otherwise
     */
    bool regCallback(fRotaryCallback callback, RotaryEvent event)
    {
      /* _cast - Casting for a higher level, it stores the callback, a callback invokation
         is casted back to it's proper type.
      */
      return TSSControl::regCallback((TSS_fControlCallback)callback, event);
    }

    /** Unregisters callback. It directs call to the base class with a cast.
     *  @param event Event for which is a callback be disabled
     *  @returns
     *    true if successful
     *    false otherwise
     */
    bool unregCallback(RotaryEvent event)
    {
      return TSSControl::unregCallback(event);
    }

    /** onInitialTouch callback is generated only if the rotary's initial
     *  touch enabler is enabled and an initial touch has been detected.
     *  @param position Actual position
     */
    virtual void onInitialTouch(uint8_t position);

    /** onAllRelease callback is generated only if the rotary's release
     *  enabler is enabled and all electrodes have been released.
     *  @param position Actual position
     */
    virtual void onAllRelease(uint8_t position);

    /** onMovement callback is generated only if the rotary's movement
     *  enabler is enabled and a movement has been detected.
     *  @param position Actual position
     */
    virtual void onMovement(uint8_t position);

    /** Macro for callback's implementation. The rotary control provides
     *  3 callbacks:
     *  AROTARY_ALL_RELEASE (callback parameters - this, AROTARY_ALL_RELEASE, position).
     *  AROTARY_INITIAL_TOUCH (callback parameters - this, AROTARY_INITIAL_TOUCH, position).
     *  AROTARY_MOVEMENT (callback parameters - this, AROTARY_MOVEMENT, position).
     */
    IMPLEMENT_CALLBACKS(3, TSS_fControlCallback, fRotaryCallback);

  private:
    /** A copy and an assigment ctor.
     */
    PROHIBIT_COPY_AND_ASSIGN(TSSRotary);
};

/** TSS Matrix control class
*/
class TSSMatrix : public TSSControl
{
  public:
    /** TSS Matrix callback events
     */
    enum MatrixEvent
    {
      MATRIX_ALL_RELEASE = 0,
      MATRIX_INITIAL_TOUCH = 1,
      MATRIX_MOVEMENT = 2,
      MATRIX_GESTURES = 3,
    };

    /** Matrix's callbacks typedef
     */
    typedef void (* fMatrixCallback)(TSSMatrix *, MatrixEvent, uint8_t, uint8_t);

    /** Initializes MATRIX control according to the TSSSystemSetup header file.
     *  @param control_num Control number (for example TSS_C0_TYPE is a control number 0).
     */
    TSSMatrix(uint8_t control_num);

    /** Disables a control in TSS.
     */
    virtual ~TSSMatrix();

    /** This is the callback handler for the matrix control.
     *  It invokes an action which is available via callbacks or public method
     *  like onInitialTouch, etc.
     */
    virtual void callbackControl(void);

    /** Enable specific flags available for the matrix control.
     *  Refer to the TSSControlFlag enum.
     *  @param flag Flag to be enabled.
     *  @returns
     *    true if flag was enabled,
     *    false otherwise.
     */
    virtual bool enable(TSSControlFlag flag);

    /** Disable specific flags available for the matrix control.
     *  Refer to the TSSControlFlag enum.
     *  @param flag Flag to be disabled.
     *  @returns
     *    true if flag was disabled,
     *    false otherwise.
     */
    virtual bool disable(TSSControlFlag flag);

    /** Set specific configuration available for the matrix control.
     *  Each control has own specific flags, please refer to the TSSControlConfig enum.
     *  @param command Command to be set.
     *  @param value Value to be set.
     *  @returns
     *    true config was set,
     *    false otherwise.
     */
    virtual bool set(TSSControlConfig command, uint8_t value);

    /** Get specific configuration available for the matrix control.
     *  Each control has own specific flags, please refer to the TSSControlConfig enum.
     *  @param command Command to be set.
     *  @returns
     *    config value,
     *    TSS_ERROR_GETCONF_ILLEGAL_CONTROL_TYPE otherwise.
     */
    virtual uint16_t get(TSSControlConfig command) const;

    /** Returns the TSS control's structure
     */
    const TSS_CSMatrix* getControlStruct(void) const
    {
      return static_cast<TSS_CSMatrix const*>(getControlStructInternal());
    }

    /** Set a pointer to the control's structure.
     */
    void setControlStruct(TSS_CSMatrix *control_struct)
    {
      setControlStructInternal(static_cast<void const *>(control_struct));
    }

    /** Registers callback. It directs call to the base class with cast.
     *  @param callback Callback to be registered
     *  @param event Event which invokes a callback
     *  @returns
     *    true if successful
     *    false otherwise
     */
    bool regCallback(fMatrixCallback callback, MatrixEvent event)
    {
      /* _cast - Casting for a higher level, it stores the callback, a callback invokation
         is casted back to it's proper type.
      */
      return TSSControl::regCallback((TSS_fControlCallback)callback, event);
    }

    /** Unregisters callback. It directs call to the base class with cast.
     *  @param event Event for which is a callback be disabled
     *  @returns
     *    true if successful
     *    false otherwise
     */
    bool unregCallback(MatrixEvent event)
    {
      return TSSControl::unregCallback(event);
    }

    /** onInitialTouch callback is generated only if the matrix's initial
     *  touch enabler is enabled and an initial touch has been detected.
     *  @param position_x Actual position on axis X
     *  @param position_y Actual position on axis Y
     */
    virtual void onInitialTouch(uint8_t position_x, uint8_t position_y);

    /** onAllRelease callback is generated only if the matrix's release
     *  enabler is enabled and all electrodes have been released.
     *  @param position_x Actual position on axis X
     *  @param position_y Actual position on axis Y
     */
    virtual void onAllRelease(uint8_t position_x, uint8_t position_y);

    /** onMovement callback is generated only if the matrix's movement
     *  enabler is enabled and a movement has been detected.
     *  @param position_x Actual position on axis X
     *  @param position_y Actual position on axis Y
     */
    virtual void onMovement(uint8_t position_x, uint8_t position_y);

    /** onGestures callback is generated only if the matrix's gestures
     *  enabler is enabled and a gesture has been detected.
     *  @param ges_distance_x Gesture distance on axis X
     *  @param ges_distance_y Gesture distance on axis Y
     */
    virtual void onGestures(uint8_t ges_distance_x, uint8_t ges_distance_y);

    /** Macro for callback's implementation. The matrix control provides
     *  4 callbacks:
     *  MATRIX_ALL_RELEASE (callback parameters - this, MATRIX_ALL_RELEASE, pos_x, pos_y).
     *  MATRIX_INITIAL_TOUCH (callback parameters - this, MATRIX_INITIAL_TOUCH, pos_x, pos_y).
     *  MATRIX_MOVEMENT (callback parameters - this, MATRIX_MOVEMENT, pos_x, pos_y).
     *  MATRIX_GESTURES (callback parameters - this, MATRIX_GESTURES, ges_distance_x, ges_distance_y).
     */
    IMPLEMENT_CALLBACKS(4, TSS_fControlCallback, fMatrixCallback);

  private:
    /** A copy and an assigment ctor.
     */
    PROHIBIT_COPY_AND_ASSIGN(TSSMatrix);
};

}   /* end of tss namespace */

#ifdef __cplusplus
  extern "C" {
#endif

  /** TSS onFault C callback which detects particular fault code. Then it invokes
   *  system's fault callbeck.
   *  @param elec_num The number of an electrode which caused fault.
   */
  void callbackFaultHelper(uint8_t elec_num);

  /** TSS C general callback for all controls. It retrieves pointer to
   *  the object which caused the callback and invokes control's callback control.
   *  @param control_id Control ID of control which caused an invokation of the
   *    callback
   */
  void callbackControlHelper(TSS_CONTROL_ID control_id);

#ifdef __cplusplus
  }
#endif



#endif

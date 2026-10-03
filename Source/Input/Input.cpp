#include "Framework.h"
#include "Input/Input.h"

#include "Application.h"
#include "Assets/TranslationKeys.h"
#include "Input/Action.h"
#include "Input/AnalogAxis.h"
#include "Input/Binding.h"
#include "Input/Event.h"
#include "Input/Recorder.h"
#include "Input/Text.h"
#include "Services/Clock.h"
#include "Services/Options.h"
#include "Services/Toasts.h"
#include "Utils/Parallel.h"
#include "Utils/Utils.h"

using namespace Silent::Assets;
using namespace Silent::Services;
using namespace Silent::Utils;

namespace Silent::Input
{
    BindingManager& InputManager::GetBindings()
    {
        return _bindings;
    }

    const Action& InputManager::GetAction(ActionId actionId) const
    {
        return _states.Actions[(int)actionId];
    }

    const AnalogAxis& InputManager::GetAnalogAxis(AnalogAxisId axisId) const
    {
        return _states.AnalogAxes[(int)axisId];
    }

    const Vector2& InputManager::GetCursorPosition() const
    {
        return _states.Device.CursorPosition;
    }

    float InputManager::GetRawEventState(EventId eventId) const
    {
        return _states.Device.Events[(int)eventId];
    }

    GamepadVendorId InputManager::GetGamepadVendorId() const
    {
        return _gamepad.VendorId;
    }

    int InputManager::GetGamepadBatteryPercentage() const
    {
        if (!IsGamepadConnected())
        {
            return NO_VALUE;
        }

        int percent = 0;
        SDL_GetGamepadPowerInfo(_gamepad.Device, &percent);
        return percent;
    }

    const Recording& InputManager::GetRecording() const
    {
        return _recorder.GetRecording();
    }

    const std::string& InputManager::GetText(const std::string& textId) const
    {
        return _text.GetText(textId);
    }

    std::vector<std::string> InputManager::GetTextLines(const std::string& bufferId, int low, int high) const
    {
        return _text.GetTextLines(bufferId, low, high);
    }

    int InputManager::GetTextCursorPosition(const std::string& textId) const
    {
        return _text.GetCursorPosition(textId);
    }

    void InputManager::SetRumble(RumbleMode mode, float intensityFrom, float intensityTo, float durationSec)
    {
        _rumble.Mode          = mode;
        _rumble.IntensityFrom = intensityFrom;
        _rumble.IntensityTo   = intensityTo;
        _rumble.DurationTicks =
        _rumble.Ticks         = SEC_TO_TICK(durationSec);
    }

    bool InputManager::IsGamepadConnected() const
    {
        return _gamepad.Id != NO_VALUE && _gamepad.Device != nullptr;
    }

    bool InputManager::IsUsingGamepad() const
    {
        return _states.Device.IsUsingGamepad;
    }

    bool InputManager::IsUsingMouse() const
    {
        return _states.Device.IsUsingMouse;
    }

    bool InputManager::HasDeviceInput() const
    {
        return _states.Device.HasKeyboardInput ||
               _states.Device.HasMouseInput    ||
               _states.Device.HasGamepadInput;
    }

    bool InputManager::HasRawActionInput() const
    {
        return _states.Device.HasRawActionInput;
    }

    bool InputManager::HasUserActionInput() const
    {
        return _states.Device.HasUserActionInput;
    }

    void InputManager::Initialize()
    {
        const auto& options = g_App.GetOptions();

        if (!SDL_Init(SDL_INIT_GAMEPAD))
        {
            Debug::Log(Fmt("Failed to initialize gamepad subsystem: {}", SDL_GetError()), Debug::LogLevel::Error);
        }

        // Initialize event states.
        _states.Device.Events.resize((int)EventId::Count);
        _states.AnalogAxes.resize((int)AnalogAxisId::Count);

        // Initialize actions.
        _states.Actions.reserve((int)ActionId::Count);
        for (int i = 0; i < (int)ActionId::Count; i++)
        {
            auto actionId = (ActionId)i;
            _states.Actions.push_back(Action(actionId));
        }

        // Initialize analog axes.
        _states.AnalogAxes.reserve((int)AnalogAxisId::Count);
        for (int i = 0; i < (int)AnalogAxisId::Count; i++)
        {
            auto axisId = (AnalogAxisId)i;
            _states.AnalogAxes.push_back(AnalogAxis(axisId));
        }

        // Initialize bindings.
        _bindings.Initialize(options->KeyboardMouseBindings, options->GamepadBindings);
    }

    void InputManager::Deinitialize()
    {
        DisconnectGamepad(_gamepad.Id);
    }

    void InputManager::Update(SDL_Window& window, const Vector2& mouseWheelAxis)
    {
        const auto& clock      = g_App.GetClock();
        const auto& translator = g_App.GetTranslator();
        auto&       executor   = g_App.GetExecutor();
        auto&       toaster    = g_App.GetToaster();

        // Clear data.
        _states.Device.HasKeyboardInput   =
        _states.Device.HasMouseInput      =
        _states.Device.HasGamepadInput    =
        _states.Device.HasRawActionInput  =
        _states.Device.HasUserActionInput = false;

        // Capture event states asynchronously.
        auto tasks = ParallelTasks
        {
            TASK(ReadKeyboard()),
            TASK(ReadMouse(window, mouseWheelAxis)),
            TASK(ReadGamepad())
        };
        executor.AddTasks(tasks).wait();

        // Update "using gamepad" state.
        if (_states.Device.HasKeyboardInput || _states.Device.HasMouseInput)
        {
            _states.Device.IsUsingGamepad = false;
        }
        else if (_states.Device.HasGamepadInput)
        {
            _states.Device.IsUsingGamepad = true;
        }

        // Update "using mouse" state.
        if (_states.Device.HasKeyboardInput || _states.Device.HasGamepadInput)
        {
            _states.Device.IsUsingMouse = false;
        }
        else if (_states.Device.HasMouseInput)
        {
            _states.Device.IsUsingMouse = true;
        }

        // Update actions and analog axes.
        UpdateActions();
        UpdateAnalogAxes();
        _recorder.Update(_states.Actions, _states.AnalogAxes);

        // Update other components.
        UpdateRumble();
        HandleHotkeyActions();

        // Warn of low gamepad battery.
        if (IsGamepadConnected())
        {
            _gamepad.BatteryWarningTicks += clock.GetTicks();
            if (TICK_TO_SEC(_gamepad.BatteryWarningTicks) >= LOW_GAMEPAD_BATTERY_WARN_INTERVAL_SEC)
            {
                _gamepad.BatteryWarningTicks = 0;

                int percent = GetGamepadBatteryPercentage();
                if (percent != NO_VALUE && percent <= LOW_GAMEPAD_BATTERY_PERCENT)
                {
                    toaster.Add(translator(KEY_SYS_GAMEPAD_BATTERY_IS_LOW));
                }
            }
        }
        else
        {
            _gamepad.BatteryWarningTicks = 0;
        }
    }

    void InputManager::ConnectGamepad(int deviceId)
    {
        constexpr int XBOX_VENDOR_CODE     = 0x45E;
        constexpr int NINTENDO_VENDOR_CODE = 0x57E;
        constexpr int SONY_VENDOR_CODE     = 0x54C;

        const auto& translator = g_App.GetTranslator();
        auto&       toaster    = g_App.GetToaster();

        // Check if a gamepad is already connected.
        if (IsGamepadConnected())
        {
            return;
        }

        // Set connection.
        _gamepad.Device = SDL_OpenGamepad(deviceId);
        if (_gamepad.Device != nullptr)
        {
            _gamepad.Id = deviceId;

            switch (SDL_GetGamepadVendor(_gamepad.Device))
            {
                case XBOX_VENDOR_CODE:
                {
                    _gamepad.VendorId = GamepadVendorId::Xbox;
                    break;
                }
                case NINTENDO_VENDOR_CODE:
                {
                    _gamepad.VendorId = GamepadVendorId::Nintendo;
                    break;
                }
                case SONY_VENDOR_CODE:
                {
                    _gamepad.VendorId = GamepadVendorId::Sony;
                    break;
                }
                default:
                {
                    _gamepad.VendorId = GamepadVendorId::Generic;
                    break;
                }
            }

            SetRumble(RumbleMode::Low, 0.0f, 1.0f, 0.1f);
            _gamepad.BatteryWarningTicks = LOW_GAMEPAD_BATTERY_WARN_INTERVAL_SEC;
            toaster.Add(translator(KEY_SYS_GAMEPAD_CONNECTED));

            Debug::Log(Fmt("{} gamepad connected.", GetGamepadVendorName(_gamepad.VendorId)));
        }
    }

    void InputManager::DisconnectGamepad(int deviceId)
    {
        const auto& translator = g_App.GetTranslator();
        auto&       toaster    = g_App.GetToaster();

        // Check if a gamepad is not connected or device IDs don't match.
        if (!IsGamepadConnected() || _gamepad.Id != deviceId)
        {
            return;
        }

        // Disconnect with toast.
        _gamepad = {};
        SDL_CloseGamepad(_gamepad.Device);
        toaster.Add(translator(KEY_SYS_GAMEPAD_DISCONNECTED));

        Debug::Log("Gamepad disconnected.");
    }

    void InputManager::LockActionGroup(ActionGroupId groupId)
    {
        for (auto actionId : ACTION_ID_GROUPS[(int)groupId])
        {
            auto& action = _states.Actions[(int)actionId];
            action.Lock();
        }
    }

    void InputManager::UnlockActions()
    {
        for (auto& action : _states.Actions)
        {
            action.Unlock();
        }
    }

    void InputManager::LockAnalogAxisGroup(AnalogAxisGroupId groupId)
    {
        for (auto axisId : ANALOG_AXIS_ID_GROUPS[(int)groupId])
        {
            auto& axis = _states.AnalogAxes[(int)axisId];
            axis.Lock();
        }
    }

    void InputManager::UnlockAnalogAxes()
    {
        for (auto& axis : _states.AnalogAxes)
        {
            axis.Unlock();
        }
    }

    void InputManager::StartPlayback(const Recording& rec)
    {
        // Lock recordable actions and analog axes.
        LockActionGroup(ActionGroupId::Recordable);
        LockAnalogAxisGroup(AnalogAxisGroupId::Recordable);

        _recorder.Play(rec,
                       ACTION_ID_GROUPS[(int)ActionGroupId::Recordable],
                       ANALOG_AXIS_ID_GROUPS[(int)AnalogAxisGroupId::Recordable]);
    }

    void InputManager::StartRecording()
    {
        _recorder.Record(ACTION_ID_GROUPS[(int)ActionGroupId::Recordable],
                         ANALOG_AXIS_ID_GROUPS[(int)AnalogAxisGroupId::Recordable]);
    }

    void InputManager::StopRecorder()
    {
        // Unlock actions and analog axes.
        UnlockActions();
        UnlockAnalogAxes();

        _recorder.Stop();
    }

    void InputManager::InsertText(const std::string& textId, int lineWidthMax, int charCountMax)
    {
        _text.InsertBuffer(textId, lineWidthMax, charCountMax);
    }
    
    void InputManager::UpdateText(const std::string& textId)
    {
        _text.UpdateBuffer(textId);
    }

    void InputManager::RemoveText(const std::string& textId)
    {
        _text.RemoveBuffer(textId);
    }

    std::string InputManager::GetGamepadVendorName(GamepadVendorId vendorId) const
    {
        constexpr char GENERIC_VENDOR_NAME[]  = "Generic";
        constexpr char XBOX_VENDOR_NAME[]     = "Xbox";
        constexpr char NINTENDO_VENDOR_NAME[] = "Nintendo";
        constexpr char SONY_VENDOR_NAME[]     = "Sony";

        switch (vendorId)
        {
            case GamepadVendorId::Generic:
            {
                break;
            }
            case GamepadVendorId::Xbox:
            {
                return XBOX_VENDOR_NAME;
            }
            case GamepadVendorId::Nintendo:
            {
                return NINTENDO_VENDOR_NAME;
            }
            case GamepadVendorId::Sony:
            {
                return SONY_VENDOR_NAME;
            }
        }

        return GENERIC_VENDOR_NAME;
    }

    void InputManager::UpdateActions()
    {
        const auto& options  = g_App.GetOptions();
        auto&       executor = g_App.GetExecutor();

        // 1) Update user action states.
        auto updateUserActions = [&]()
        {
            // Get active action binding profiles.
            const auto& kmProfile      = _bindings.GetProfile(options->ActiveKeyboardMouseProfileId);
            const auto& gamepadProfile = _bindings.GetProfile(options->ActiveGamepadProfileId);

            for (auto actionGroupId : USER_ACTION_GROUP_IDS)
            {
                const auto& actionIds = ACTION_ID_GROUPS[(int)actionGroupId];
                for (auto actionId : actionIds)
                {
                    auto& action = _states.Actions[(int)actionId];
                    if (action.IsLocked())
                    {
                        continue;
                    }

                    float state = 0.0f;

                    // Get max gamepad event state.
                    if (IsGamepadConnected())
                    {
                        const auto& gamepadEventIds = gamepadProfile.at(actionId);
                        for (const auto& eventId : gamepadEventIds)
                        {
                            state = std::max(state, GetRawEventState(eventId));
                        }
                    }

                    // If no valid gamepad event state exists, get max keyboard/mouse event state.
                    if (state == 0.0f)
                    {
                        const auto& kmEventIds = kmProfile.at(actionId);
                        for (const auto& eventId : kmEventIds)
                        {
                            state = std::max(state, GetRawEventState(eventId));
                        }
                    }

                    // Use max bound event state.
                    action.Update(state);
                    if (state != 0.0f)
                    {
                        _states.Device.HasUserActionInput = true;
                    }
                }
            }
        };

        // 2) Update raw action states.
        auto updateRawActions = [&]()
        {
            for (auto profileId : RAW_BINDING_PROFILE_IDS)
            {
                const auto& profile = _bindings.GetProfile(profileId);
                for (auto& [keyActionId, eventIds] : profile)
                {
                    auto& action = _states.Actions[(int)keyActionId];
                    if (action.IsLocked())
                    {
                        continue;
                    }

                    float state = 0.0f;

                    for (auto eventId : eventIds)
                    {
                        state = std::max(state, GetRawEventState(eventId));
                    }

                    // Use max bound event state.
                    action.Update(state);
                    if (state != 0.0f)
                    {
                        _states.Device.HasRawActionInput;
                    }
                }
            }
        };

        // Update action states asynchronously.
        auto tasks = ParallelTasks
        {
            TASK(updateUserActions()),
            TASK(updateRawActions())
        };
        executor.AddTasks(tasks).wait();
    }

    void InputManager::UpdateAnalogAxes()
    {
        const auto& options = g_App.GetOptions();

        auto&       moveAxis       = _states.AnalogAxes[(int)AnalogAxisId::Move];
        auto&       camAxis        = _states.AnalogAxes[(int)AnalogAxisId::Camera];
        const auto& mouseAxis      = _states.AnalogAxes[(int)AnalogAxisId::Mouse];
        const auto& leftStickAxis  = _states.AnalogAxes[(int)AnalogAxisId::StickLeft];
        const auto& rightStickAxis = _states.AnalogAxes[(int)AnalogAxisId::StickRight];

        // Set move axis state.
        if (!moveAxis.IsLocked())
        {
            if (leftStickAxis.State != Vector2::Zero)
            {
                // Analog.
                moveAxis = leftStickAxis;
            }
            else
            {
                // Discrete `Up`/`Down`.
                if (( GetAction(In::Up).IsHeld() &&  GetAction(In::Down).IsHeld()) ||
                    (!GetAction(In::Up).IsHeld() && !GetAction(In::Down).IsHeld()))
                {
                    moveAxis.State.y = 0.0f;
    
                }
                else if (GetAction(In::Up).IsHeld())
                {
                    moveAxis.State.y = 1.0f;
                }
                else if (GetAction(In::Down).IsHeld())
                {
                    moveAxis.State.y = -1.0f;
                }
    
                // Discrete `Left`/`Right`.
                if (( GetAction(In::Left).IsHeld() &&  GetAction(In::Right).IsHeld()) ||
                    (!GetAction(In::Left).IsHeld() && !GetAction(In::Right).IsHeld()))
                {
                    moveAxis.State.x = 0.0f;
    
                }
                else if (GetAction(In::Left).IsHeld())
                {
                    moveAxis.State.x = -1.0f;
                }
                else if (GetAction(In::Right).IsHeld())
                {
                    moveAxis.State.x = 1.0f;
                }
            }
        }

        // Set camera axis state.
        if (!camAxis.IsLocked())
        {
            if (rightStickAxis.State != Vector2::Zero)
            {
                camAxis = rightStickAxis;
            }
            else
            {
                camAxis = mouseAxis;
            }
        }
    }

    void InputManager::UpdateRumble()
    {
        const auto& options = g_App.GetOptions();

        if (!IsGamepadConnected() || !options->EnableVibration || _rumble.Ticks == 0)
        {
            _rumble = {};
            return;
        }

        // Compute intensity.
        float alpha     = (float)_rumble.Ticks / (float)_rumble.DurationTicks;
        float intensity = std::lerp(_rumble.IntensityFrom, _rumble.IntensityTo, alpha);

        // Compute frequencies.
        bool   hasLowFreq  = _rumble.Mode == RumbleMode::Low  || _rumble.Mode == RumbleMode::LowAndHigh;
        bool   hasHighFreq = _rumble.Mode == RumbleMode::High || _rumble.Mode == RumbleMode::LowAndHigh;
        ushort freqLow     = hasLowFreq  ? (ushort)(intensity * USHRT_MAX) : 0;
        ushort freqHigh    = hasHighFreq ? (ushort)(intensity * USHRT_MAX) : 0;

        // Compute duration.
        int durationMs = (int)roundf(TICK_TO_SEC(_rumble.DurationTicks) * 1000);

        // Rumble gamepad.
        if (!SDL_RumbleGamepad(_gamepad.Device, freqLow, freqHigh, durationMs))
        {
            Debug::Log(Fmt("Failed to rumble gamepad: {}", SDL_GetError()), Debug::LogLevel::Error);
        }

        _rumble.Ticks--;
    }

    void InputManager::ReadKeyboard()
    {
        constexpr auto START_EVENT_ID = EventId::A;

        int eventIdx = (int)START_EVENT_ID;

        // Set keyboard key event states.
        int        keyboardStateCount = 0;
        const auto keyboardState      = ToSpan(SDL_GetKeyboardState(&keyboardStateCount), SDL_SCANCODE_COUNT);
        for (auto scanCode : VALID_KEYBOARD_SCAN_CODES)
        {
            if (scanCode < keyboardStateCount)
            {
                bool state = keyboardState[scanCode];
                if (state)
                {
                    _states.Device.HasKeyboardInput = true;
                }

                _states.Device.Events[eventIdx] = state ? 1.0f : 0.0f;
            }

            eventIdx++;
        }

        // Set keyboard modifier event states.
        auto modState = SDL_GetModState();
        for (int modCode : VALID_KEYBOARD_MODIFIER_CODES)
        {
            bool state = modState & modCode;
            if (state)
            {
                _states.Device.HasKeyboardInput = true;
            }

            _states.Device.Events[eventIdx] = state ? 1.0f : 0.0f;
            eventIdx++;
        }
    }

    void InputManager::ReadMouse(SDL_Window& window, const Vector2& wheelAxis)
    {
        constexpr auto START_EVENT_ID = EventId::MouseClickLeft;

        const auto& options  = g_App.GetOptions();
        const auto& renderer = g_App.GetRenderer();

        int eventIdx = (int)START_EVENT_ID;

        // Compute cursor position.
        auto pos      = Vector2::Zero;
        auto butState = SDL_GetMouseState(&pos.x, &pos.y);
        pos           = (pos / renderer.GetViewportResolution().ToVector2()) * SCREEN_SPACE_RES;

        // Set mouse button event states.
        for (int butCode : VALID_MOUSE_BUTTON_CODES)
        {
            bool state = butState & SDL_BUTTON_MASK(butCode);
            if (state)
            {
                _states.Device.HasMouseInput = true;
            }

            _states.Device.Events[eventIdx] = state ? 1.0f : 0.0f;
            eventIdx++;
        }

        if (wheelAxis != Vector2::Zero)
        {
            _states.Device.HasMouseInput = true;
        }

        // @todo Investigate. Unclear how SDL3 mouse wheel values work.
        // Set mouse scroll event states.
        _states.Device.Events[eventIdx]     = (wheelAxis.x < 0.0f) ? std::clamp(abs(wheelAxis.x), 0.0f, 1.0f) : 0.0f;
        _states.Device.Events[eventIdx + 1] = (wheelAxis.x > 0.0f) ? std::clamp(abs(wheelAxis.x), 0.0f, 1.0f) : 0.0f;
        _states.Device.Events[eventIdx + 2] = (wheelAxis.y < 0.0f) ? std::clamp(abs(wheelAxis.y), 0.0f, 1.0f) : 0.0f;
        _states.Device.Events[eventIdx + 3] = (wheelAxis.y > 0.0f) ? std::clamp(abs(wheelAxis.y), 0.0f, 1.0f) : 0.0f;
        eventIdx                           += SQUARE(Vector2::AXIS_COUNT);

        // Set cursor position state.
        auto prevCursorPos            = _states.Device.CursorPosition;
        _states.Device.CursorPosition = pos;

        auto res = Vector2i::Zero;
        if (!SDL_GetWindowSize(&window, &res.x, &res.y))
        {
            Debug::Log(Fmt("Failed to get window size: {}", SDL_GetError()), Debug::LogLevel::Error);
        }

        float sensitivity   = (options->MouseSensitivity * 0.1f) + 0.4f;
        auto  moveAxisState = (((_states.Device.CursorPosition - prevCursorPos) / SCREEN_SPACE_RES) *
                               (res.ToVector2() / SCREEN_SPACE_RES)) * sensitivity;
        if (moveAxisState != Vector2::Zero)
        {
            _states.Device.HasMouseInput = true;
        }

        // Set mouse movement event states.
        _states.Device.Events[eventIdx]     = (moveAxisState.x < 0.0f) ? abs(moveAxisState.x) : 0.0f;
        _states.Device.Events[eventIdx + 1] = (moveAxisState.x > 0.0f) ? abs(moveAxisState.x) : 0.0f;
        _states.Device.Events[eventIdx + 2] = (moveAxisState.y < 0.0f) ? abs(moveAxisState.y) : 0.0f;
        _states.Device.Events[eventIdx + 3] = (moveAxisState.y > 0.0f) ? abs(moveAxisState.y) : 0.0f;
        eventIdx                           += SQUARE(Vector2::AXIS_COUNT);

        // Set raw mouse axis state.
        auto& mouseAxis = _states.AnalogAxes[(int)AnalogAxisId::Mouse];
        if (!mouseAxis.IsLocked())
        {
            mouseAxis.State = moveAxisState;
        }
    }

    void InputManager::ReadGamepad()
    {
        constexpr auto  START_GAMEPAD_EVENT_ID = EventId::GamepadSouth;
        constexpr float AXIS_DEADZONE          = ((float)SHRT_MAX / 8.0f) / (float)SHRT_MAX;

        int eventIdx = (int)START_GAMEPAD_EVENT_ID;

        // Set gamepad button event states.
        for (auto butCode : VALID_GAMEPAD_BUTTON_CODES)
        {
            bool state = false;
            if (IsGamepadConnected())
            {
                state = SDL_GetGamepadButton(_gamepad.Device, butCode);
            }
            if (state)
            {
                _states.Device.HasGamepadInput = true;
            }

            _states.Device.Events[eventIdx] = state ? 1.0f : 0.0f;
            eventIdx++;
        }

        // Collect stick axes.
        auto stickAxisStates = std::vector<Vector2>(VALID_GAMEPAD_STICK_AXIS_CODES.size() / Vector2::AXIS_COUNT);
        for (int i = 0, j = 0; i < VALID_GAMEPAD_STICK_AXIS_CODES.size(); i++)
        {
            if (!IsGamepadConnected())
            {
                break;
            }

            auto  axisCode = VALID_GAMEPAD_STICK_AXIS_CODES[i];
            float state    = (float)SDL_GetGamepadAxis(_gamepad.Device, axisCode) / (float)SHRT_MAX;

            auto& axis = stickAxisStates[j];
            if ((i % Vector2::AXIS_COUNT) == 0)
            {
                axis.x = state;
            }
            else
            {
                axis.y = state;

                // Remap axis to active range.
                if (axis.Length() >= AXIS_DEADZONE)
                {
                    float remappedLength = Remap(axis.Length(), AXIS_DEADZONE, 1.0f, 0.0f, 1.0f);
                    axis                 = Vector2::Normalize(axis) * remappedLength;
                }
                else
                {
                    axis = Vector2::Zero;
                }

                j++;
            }
        }

        // Set gamepad stick axis event states.
        for (int i = 0; i < stickAxisStates.size(); i++)
        {
            const auto& axis = stickAxisStates[i];
            if (axis != Vector2::Zero)
            {
                _states.Device.HasGamepadInput = true;
            }

            _states.Device.Events[eventIdx + i]       = (axis.x < 0.0f) ? abs(axis.x) : 0.0f;
            _states.Device.Events[eventIdx + (i + 1)] = (axis.x > 0.0f) ? abs(axis.x) : 0.0f;
            _states.Device.Events[eventIdx + (i + 2)] = (axis.y < 0.0f) ? abs(axis.y) : 0.0f;
            _states.Device.Events[eventIdx + (i + 3)] = (axis.y > 0.0f) ? abs(axis.y) : 0.0f;
            eventIdx                                 += Vector2::AXIS_COUNT * 2;
        }

        // Set gamepad trigger axis event states.
        for (auto axisCode : VALID_GAMEPAD_TRIGGER_AXIS_CODES)
        {
            float state = 0.0f;
            if (IsGamepadConnected())
            {
                // Remap state to active range.
                state = (float)SDL_GetGamepadAxis(_gamepad.Device, axisCode) / (float)SHRT_MAX;
                if (state >= AXIS_DEADZONE)
                {
                    state = Remap(state, AXIS_DEADZONE, 1.0f, 0.0f, 1.0f);
                }
            }
            if (state > 0.0f)
            {
                _states.Device.HasGamepadInput = true;
            }

            _states.Device.Events[eventIdx] = state;
            eventIdx++;
        }

        // Set raw left stick axis state.
        auto& leftStickAxis = _states.AnalogAxes[(int)AnalogAxisId::StickLeft];
        if (!leftStickAxis.IsLocked())
        {
            leftStickAxis.State = stickAxisStates.front();
        }

        // Set raw right stick axis state.
        auto& rightStickAxis = _states.AnalogAxes[(int)AnalogAxisId::StickRight];
        if (!rightStickAxis.IsLocked())
        {
            rightStickAxis.State = stickAxisStates.back();
        }
    }

    void InputManager::HandleHotkeyActions()
    {
        // Capture screenshot.
        static bool dbScreenshot = true;
        if ((GetRawEventState(EventId::PrintScreen) || GetRawEventState(EventId::F12)) && dbScreenshot)
        {
            const auto& renderer = g_App.GetRenderer();
            renderer.SaveScreenshot();
        }
        dbScreenshot = !(GetRawEventState(EventId::PrintScreen) || GetRawEventState(EventId::F12));

        // Toggle fullscreen.
        static bool dbFullscreen = true;
        if (((GetRawEventState(EventId::Alt) && GetRawEventState(EventId::Return)) || GetRawEventState(EventId::F11)) &&
            dbFullscreen)
        {
            g_App.ToggleFullscreen();
        }
        dbFullscreen = !((GetRawEventState(EventId::Alt) && GetRawEventState(EventId::Return)) ||
                         GetRawEventState(EventId::F11));

        auto& options = g_App.GetOptions();
        if (options->EnableDebugMode)
        {
            // Toggle debug menu.
            static bool dbDebugGui = true;
            if (GetRawEventState(EventId::Grave) && dbDebugGui)
            {
                g_App.ToggleDebugMenu();
            }
            dbDebugGui = !GetRawEventState(EventId::Grave);
        }
    }
}

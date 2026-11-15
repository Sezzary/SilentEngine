#include "Framework.h"
#include "Input/Recorder.h"

#include "Input/Action.h"
#include "Input/AnalogAxis.h"

namespace Silent::Input
{
    RecorderState Recorder::GetState() const
    {
        return _state;
    }

    const Recording& Recorder::GetRecording() const
    {
        return _recording;
    }

    void Recorder::Update(std::vector<Action>& actions, std::vector<AnalogAxis>& axes)
    {
        switch (_state)
        {
            default:
            case RecorderState::None:                                           break;
            case RecorderState::Playing:       StatePlaying(actions, axes);     break;
            case RecorderState::Recording:     StateRecording(actions, axes);   break;
            case RecorderState::StopPlaying:   StateStopPlaying(actions, axes); break;
            case RecorderState::StopRecording: StateStopRecording();            break;
        }
    }

    void Recorder::Play(const Recording& rec,
                        const std::vector<ActionId>& validActionIds, const std::vector<AnalogAxisId>& validAxisIds)
    {
        // Check if recorder is idle.
        if (_state != RecorderState::None)
        {
            Debug::Log(Fmt("Attempted to start input recording playback while in non-idle state {}.", (int)_state),
                       Debug::LogLevel::Error);
            return;
        }

        // Check if recording is valid.
        if (rec.FrameCount == NO_VALUE)
        {
            Debug::Log(Fmt("Attempted to play invalid input recording."), Debug::LogLevel::Error);
            return;
        }

        // Setup.
        _state              = RecorderState::Playing;
        _frameIdx           = 0;
        _keyframeIdx        = 0;
        _recording          = rec;
        _validActionIds     = validActionIds;
        _validAnalogAxisIds = validAxisIds;
        InitializeActiveFrame();

        Debug::Log("Started input recording playback.");
    }

    void Recorder::Record(const std::vector<ActionId>& validActionIds, const std::vector<AnalogAxisId>& validAxisIds)
    {
        // Check if recorder is idle.
        if (_state != RecorderState::None)
        {
            Debug::Log(Fmt("Attempted to interrupt input recorder with new recording while in state {}.", (int)_state),
                       Debug::LogLevel::Error);
            return;
        }

        // Setup.
        _state              = RecorderState::Recording;
        _frameIdx           = 0;
        _keyframeIdx        = 0;
        _recording          = {};
        _validActionIds     = validActionIds;
        _validAnalogAxisIds = validAxisIds;
        InitializeActiveFrame();

        Debug::Log("Started input recording.");
    }

    void Recorder::Stop()
    {
        switch (_state)
        {
            case RecorderState::Playing:
            {
                _state = RecorderState::StopPlaying;
                break;
            }
            case RecorderState::Recording:
            {
                _state = RecorderState::StopRecording;
                break;
            }
            default:
            {
                Debug::Log(Fmt("Attempted to stop input recorder while in state {}.", (int)_state),
                           Debug::LogLevel::Error);
                break;
            }
        }
    }

    RecordedAction* Recorder::GetActiveActionEvent(ActionId actionId)
    {
        for (auto& activeEvent : _activeFrame.Actions)
        {
            if (activeEvent.ActionId == actionId)
            {
                return &activeEvent;
            }
        }

        return nullptr;
    }

    RecordedAnalogAxis* Recorder::GetActiveAnalogAxisEvent(AnalogAxisId axisId)
    {
        for (auto& activeEvent : _activeFrame.AnalogAxes)
        {
            if (activeEvent.AnalogAxisId == axisId)
            {
                return &activeEvent;
            }
        }

        return nullptr;
    }

    void Recorder::InitializeActiveFrame()
    {
        _activeFrame = {};

        // Set action event states.
        for (auto actionId : _validActionIds)
        {
            _activeFrame.Actions.push_back(RecordedAction
            {
                .ActionId = actionId,
                .State    = 0.0f
            });
        }

        // Set analog axis event states.
        for (auto axisId : _validAnalogAxisIds)
        {
            _activeFrame.AnalogAxes.push_back(RecordedAnalogAxis
            {
                .AnalogAxisId = axisId,
                .State        = Vector2::Zero
            });
        }
    }

    void Recorder::ClearActionsAndAnalogAxes(std::vector<Action>& actions, std::vector<AnalogAxis>& axes)
    {
        // Clear valid actions.
        for (auto actionId : _validActionIds)
        {
            auto& action = actions[(int)actionId];
            action.Clear();
        }

        // Clear valid analog axes.
        for (auto axisId : _validAnalogAxisIds)
        {
            auto& axis = axes[(int)axisId];
            axis.State = Vector2::Zero;
        }
    }

    void Recorder::StatePlaying(std::vector<Action>& actions, std::vector<AnalogAxis>& axes)
    {
        // Check if playback is finished.
        if (_frameIdx >= _recording.FrameCount)
        {
            _state = RecorderState::StopPlaying;
            return;
        }

        // Clear actions and analog axes on initial frame.
        if (_frameIdx == 0)
        {
            ClearActionsAndAnalogAxes(actions, axes);
        }

        // Update active frame.
        if (_keyframeIdx < _recording.Keyframes.size())
        {
            const auto& nextKeyframe = _recording.Keyframes[_keyframeIdx];
            if (nextKeyframe.FrameIdx == _frameIdx)
            {
                // Update active action events.
                for (const auto& nextEvent : nextKeyframe.Actions)
                {
                    auto* activeEvent = GetActiveActionEvent(nextEvent.ActionId);
                    if (activeEvent != nullptr)
                    {
                        activeEvent->State = nextEvent.State;
                    }
                }

                // Update active analog axis events.
                for (const auto& nextEvent : nextKeyframe.AnalogAxes)
                {
                    auto* activeEvent = GetActiveAnalogAxisEvent(nextEvent.AnalogAxisId);
                    if (activeEvent != nullptr)
                    {
                        activeEvent->State = nextEvent.State;
                    }
                }

                // Increment active keyframe index.
                _keyframeIdx++;
            }
        }

        // Update locked action states.
        for (const auto& activeEvent : _activeFrame.Actions)
        {
            auto& action = actions[(int)activeEvent.ActionId];
            if (action.IsLocked())
            {
                action.Update(activeEvent.State);
            }
        }

        // Update locked analog axis states.
        for (const auto& activeEvent : _activeFrame.AnalogAxes)
        {
            auto& axis = axes[(int)activeEvent.AnalogAxisId];
            if (axis.IsLocked())
            {
                axis.State = activeEvent.State;
            }
        }

        // Increment active playback frame index.
        _frameIdx++;
    }

    void Recorder::StateRecording(std::vector<Action>& actions, std::vector<AnalogAxis>& axes)
    {
        _activeFrame.FrameIdx = _frameIdx;

        // Clear actions and analog axes on initial frame.
        if (_frameIdx == 0)
        {
            ClearActionsAndAnalogAxes(actions, axes);
        }

        auto recKeyframe = RecordedKeyframe{};

        // Run through valid action IDs.
        for (const auto actionId : _validActionIds)
        {
            const auto& action = actions[(int)actionId];
            
            auto* activeEvent = GetActiveActionEvent(action.GetId());
            if (activeEvent == nullptr)
            {
                continue;
            }

            // Record new action event if recording initial keyframe or action state changed.
            if (_frameIdx == 0 || activeEvent->State != action.GetState())
            {
                recKeyframe.Actions.push_back(RecordedAction
                {
                    .ActionId = action.GetId(),
                    .State    = action.GetState()
                });

                // Update active event.
                activeEvent->State = action.GetState();
            }
        }

        // Run through valid analog axis IDs.
        for (auto axisId : _validAnalogAxisIds)
        {
            const auto& axis = axes[(int)axisId];

            auto* activeEvent = GetActiveAnalogAxisEvent(axis.GetId());
            if (activeEvent == nullptr)
            {
                continue;
            }

            // Record new analog axis event if recording initial keyframe or analog axis state changed.
            if (_frameIdx == 0 || activeEvent->State != axis.State)
            {
                recKeyframe.AnalogAxes.push_back(RecordedAnalogAxis
                {
                    .AnalogAxisId = axis.GetId(),
                    .State        = axis.State
                });

                // Update active event.
                activeEvent->State = axis.State;
            }
        }

        // Collect new recorded keyframe.
        if (!recKeyframe.Actions.empty() || !recKeyframe.AnalogAxes.empty())
        {
            recKeyframe.FrameIdx = _frameIdx;
            _recording.Keyframes.push_back(std::move(recKeyframe));
        }

        // Increment playback frame count and active playback frame index.
        _recording.FrameCount++;
        _frameIdx++;
    }

    void Recorder::StateStopPlaying(std::vector<Action>& actions, std::vector<AnalogAxis>& axes)
    {
        // Update action states.
        for (const auto& activeEvent : _activeFrame.Actions)
        {
            auto& action = actions[(int)activeEvent.ActionId];
            action.Clear();
        }

        // Update analog axis states.
        for (const auto& activeEvent : _activeFrame.AnalogAxes)
        {
            auto& axis = axes[(int)activeEvent.AnalogAxisId];
            axis.State = Vector2::Zero;
        }

        _state = RecorderState::None;
        Debug::Log("Stopped input recording playback.");
    }

    void Recorder::StateStopRecording()
    {
        _state = RecorderState::None;
        Debug::Log("Stopped input recording.");
    }
}

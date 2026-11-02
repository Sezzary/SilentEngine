#include "Framework.h"
#include "Renderer/Common/View.h"

#include "Application.h"
#include "Input/Input.h"

using namespace Silent::Input;

namespace Silent::Renderer
{
    View::View(const Vector3& pos, const Vector2i& size)
    {
        Position = pos;
        Size     = size;
    }

    Matrix View::GetMatrix(float fov, float aspect, float nearPlane, float farPlane)
    {
        constexpr float TARGET_DIST = 3.0f;

        auto viewMat = Matrix::CreateLookAt(Position, Position + (Direction * TARGET_DIST), Up);
        auto projMat = Matrix::CreatePerspective(Fov, aspect, nearPlane, farPlane);
        return projMat * viewMat;
    }

    void View::Move()
    {
        const auto& input = g_App.GetInput();

        // Modulate speed.
        static float speed = 0.1f;
        if (input.GetAction(In::Shift).IsHeld())
        {
            speed = 0.2f;
        }
        else if (input.GetAction(In::Z).IsHeld())
        {
            speed = 0.01f;
        }

        // Move on 2D plane.
        if (input.GetAction(In::MouseScrollDown).IsHeld() || input.GetAction(In::W).IsHeld())
        {
            Position.Translate(Direction, speed);
        }
        if (input.GetAction(In::A).IsHeld())
        {
            Position.Translate(Vector3::Normalize(Vector3::Cross(Direction, Up)), -speed);
        }
        if (input.GetAction(In::MouseScrollUp).IsHeld() || input.GetAction(In::S).IsHeld())
        {
            Position.Translate(Direction, -speed);
        }
        if (input.GetAction(In::D).IsHeld())
        {
            Position.Translate(Vector3::Normalize(Vector3::Cross(Direction, Up)), speed);
        }

        // Move vertically.
        if (input.GetAction(In::Space).IsHeld())
        {
            Position.Translate(Up, speed);
        }
        if (input.GetAction(In::Ctrl).IsHeld())
        {
            Position.Translate(Up, -speed);
        }

        // Pan.
        if (input.GetAction(In::MouseClickMiddle).IsHeld())
        {
            const auto& mouseAxis = input.GetAnalogAxis(AnalogAxisId::Mouse);
            Position.Translate(Vector3::Normalize(Vector3::Cross(Direction, Up)), -mouseAxis.State.x);
            Position.Translate(Up, mouseAxis.State.y);
        }

        // Rotate.
        const auto& mouseAxis = input.GetAnalogAxis(AnalogAxisId::Mouse);
        if (input.GetAction(In::Q).IsHeld())
        {
            auto rot  = EulerAngles(Q12_ANGLE(-mouseAxis.State.y), Q12_ANGLE(mouseAxis.State.x), Q12_ANGLE(0.0f));
            Direction = Vector3::Transform(Direction, rot.ToRotationMatrix());
        }

        // Change FOV.
        if (input.GetAction(In::MouseScrollDown).IsClicked())
        {
            Fov -= 1.0f;
            if (Fov < 1.0f)
            {
                Fov = 1.0f;
            }
        }
        else if (input.GetAction(In::MouseScrollUp).IsClicked())
        {
            Fov += 1.0f;
            if (Fov < 45.0f)
            {
                Fov = 45.0f;
            }
        }
    }
}

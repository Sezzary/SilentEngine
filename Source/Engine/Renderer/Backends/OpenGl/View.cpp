#include "Framework.h"
#include "Engine/Renderer/Backends/OpenGl/View.h"

#include "Engine/Application.h"
#include "Engine/Renderer/Backends/OpenGl/ShaderProgram.h"
#include "Engine/Services/Input/Input.h"

using namespace Silent::Input;

namespace Silent::Renderer
{
    View::View(const Vector3& pos, const Vector2i& size)
    {
        Position = pos;
        Size     = size;
    }

    void View::ExportMatrix(float fov, float aspect, float nearPlane, float farPlane, ShaderProgram& shaderProg, const std::string& uniName)
    {
        auto viewMat = Matrix::CreateLookAt(Position, Position + Direction * 3, Up);
        auto projMat = Matrix::CreatePerspective(Fov, aspect, nearPlane, farPlane);
        shaderProg.SetMatrix(uniName, projMat * viewMat);
    }

    void View::Move()
    {
        const auto& input = g_App.GetInput();

        // Modulate speed.
        if (input.GetAction(In::Shift).IsHeld())
        {
            Speed = 0.1f;
        }
        else
        {
            Speed = 0.05f;
        }

        // Move on 2D plane.
        if (input.GetAction(In::W).IsHeld())
        {
            Position.Translate(Direction, Speed);
        }
        if (input.GetAction(In::A).IsHeld())
        {
            Position.Translate(Vector3::Normalize(Vector3::Cross(Direction, Up)), -Speed);
        }
        if (input.GetAction(In::S).IsHeld())
        {
            Position.Translate(Direction, -Speed);
        }
        if (input.GetAction(In::D).IsHeld())
        {
            Position.Translate(Vector3::Normalize(Vector3::Cross(Direction, Up)), Speed);
        }

        // Move vertically.
        if (input.GetAction(In::Space).IsHeld())
        {
            Position.Translate(Up, Speed);
        }
        if (input.GetAction(In::Ctrl).IsHeld())
        {
            Position.Translate(Up, -Speed);
        }

        // Pan.
        if (input.GetAction(In::MouseClickMiddle).IsHeld())
        {
            const auto& mouseAxis = input.GetAnalogAxis(AnalogAxisId::Mouse);
            Position.Translate(Vector3::Normalize(Vector3::Cross(Direction, Up)), -mouseAxis.x);
            Position.Translate(Up, mouseAxis.y);
        }

        // Rotate.
        /*const auto& mouseAxis = input.GetAnalogAxis(AnalogAxisId::Mouse);
        if (mouseAxis != Vector2::Zero)
        {
            static auto rot = EulerAngles::Identity;
            rot            += EulerAngles(FP_ANGLE(mouseAxis.x), FP_ANGLE(mouseAxis.y), 0);
            Direction       = rot.ToDirection();
        }*/

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

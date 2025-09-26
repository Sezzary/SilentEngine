#include "Framework.h"
#include "Engine/Renderer/Backends/OpenGl/ShaderProgram.h"

#include "Engine/Renderer/Backends/OpenGl/ElementBuffer.h"
#include "Engine/Renderer/Backends/OpenGl/VertexArray.h"
#include "Engine/Renderer/Backends/OpenGl/VertexBuffer.h"

namespace Silent::Renderer
{
    ShaderProgram::ShaderProgram(const std::string& name)
    {
        constexpr char VERT_FILE_EXT[] = ".vert";
        constexpr char FRAG_FILE_EXT[] = ".frag";

        // Set name.
        _name = name;

        // Read source files as strings.
        auto vertCode = GetFileContents("Shaders/" + name + VERT_FILE_EXT);
        auto fragCode = GetFileContents("Shaders/" + name + FRAG_FILE_EXT);

        const char* vertSrc = vertCode.c_str();
        const char* fragSrc = fragCode.c_str();

        // Compile vertex shader.
        uint vertShaderId = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertShaderId, 1, &vertSrc, nullptr);
        glCompileShader(vertShaderId);
        LogShaderError(vertShaderId, "VERTEX");

        // Compile fragment shader.
        uint fragShaderId = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragShaderId, 1, &fragSrc, nullptr);
        glCompileShader(fragShaderId);
        LogShaderError(vertShaderId, "FRAGMENT");

        // Create shader program.
        _programId = glCreateProgram();
        glAttachShader(_programId, vertShaderId);
        glAttachShader(_programId, fragShaderId);
        glLinkProgram(_programId);
        LogProgramError(_programId);

        // Cleanup.
        glDeleteShader(vertShaderId);
        glDeleteShader(fragShaderId);
    }

    const std::string& ShaderProgram::GetName() const
    {
        return _name;
    }

    uint ShaderProgram::GetId() const
    {
        return _programId;
    }

    void ShaderProgram::SetBool(const std::string& uniName, bool val) const
    {
        glUniform1i(GetUniformLocation(uniName), (int)val);
    }

    void ShaderProgram::SetInt(const std::string& uniName, int val) const
    {
        glUniform1i(GetUniformLocation(uniName), val);
    }

    void ShaderProgram::SetUint(const std::string& uniName, uint val) const
    {
        glUniform1ui(GetUniformLocation(uniName), val);
    }

    void ShaderProgram::SetFloat(const std::string& uniName, float val) const
    {
        glUniform1f(GetUniformLocation(uniName), val);
    }

    void ShaderProgram::SetVector2(const std::string& uniName, const Vector2& val) const
    {
        glUniform2fv(GetUniformLocation(uniName), 1, &val[0]);
    }

    void ShaderProgram::SetVector3(const std::string& uniName, const Vector3& val) const
    {
        glUniform3fv(GetUniformLocation(uniName), 1, &val[0]);
    }

    void ShaderProgram::SetVector4(const std::string& uniName, const Vector4& val) const
    {
        glUniform4fv(GetUniformLocation(uniName), 1, &val[0]);
    }

    void ShaderProgram::SetMatrix(const std::string& uniName, const Matrix& val) const
    {
        glUniformMatrix4fv(GetUniformLocation(uniName), 1, false, &val[0][0]);
    }

    void ShaderProgram::SetIntArray(const std::string& uniName, const std::span<int>& val) const
    {
        glUniform1iv(GetUniformLocation(uniName), val.size(), val.data());
    }

    void ShaderProgram::SetUintArray(const std::string& uniName, const std::span<uint>& val) const
    {
        glUniform1uiv(GetUniformLocation(uniName), val.size(), val.data());
    }

    void ShaderProgram::SetFloatArray(const std::string& uniName, const std::span<float>& val) const
    {
        glUniform1fv(GetUniformLocation(uniName), val.size(), val.data());
    }

    void ShaderProgram::SetVector2Array(const std::string& uniName, const std::span<Vector2>& val) const
    {
        glUniform2fv(GetUniformLocation(uniName), val.size(), (const float*)val.data());
    }

    void ShaderProgram::SetVector3Array(const std::string& uniName, const std::span<Vector3>& val) const
    {
        glUniform3fv(GetUniformLocation(uniName), val.size(), (const float*)val.data());
    }

    void ShaderProgram::SetVector4Array(const std::string& uniName, const std::span<Vector4>& val) const
    {
        glUniform4fv(GetUniformLocation(uniName), val.size(), (const float*)val.data());
    }

    void ShaderProgram::SetMatrixArray(const std::string& uniName, const std::span<Matrix>& val) const
    {
        glUniformMatrix4fv(GetUniformLocation(uniName), 1, false, (const float*)val.data());
    }

    void ShaderProgram::Activate() const
    {
        glUseProgram(_programId);
    }

    void ShaderProgram::Delete() const
    {
        glDeleteProgram(_programId);
    }

    std::string ShaderProgram::GetFileContents(const std::string& filename) const
    {
        auto input = std::ifstream(filename, std::ios::binary);
        if (!input)
        {
            throw std::runtime_error("Failed to open shader file '" + filename + "`.");
        }

        auto contents = std::string();
        input.seekg(0, std::ios::end);
        contents.resize(input.tellg());
        input.seekg(0, std::ios::beg);
        input.read(&contents[0], contents.size());
        input.close();
        return contents;
    }

    int ShaderProgram::GetUniformLocation(const std::string& uniName) const
    {
        int uniLoc = glGetUniformLocation(_programId, uniName.c_str());
        if constexpr (IS_DEBUG_BUILD)
        {
            if (uniLoc == NO_VALUE)
            {
                Log("Attempted to set invalid uniform '" + uniName + "'.", LogLevel::Warning, LogMode::Debug);
            }
        }

        return uniLoc;
    }

    void ShaderProgram::LogShaderError(uint shaderId, const std::string& type) const
    {
        constexpr uint MSG_BUFFER_SIZE = 512;

        int  success = 0;
        char msgBuffer[MSG_BUFFER_SIZE];

        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shaderId, MSG_BUFFER_SIZE, nullptr, msgBuffer);
            Log("Failed to compile `" + type + "` shader: " + msgBuffer, LogLevel::Error);
        }
    }

    void ShaderProgram::LogProgramError(uint progId) const
    {
        constexpr uint MSG_BUFFER_SIZE = 512;

        int  success = 0;
        char msgBuffer[MSG_BUFFER_SIZE];

        glGetProgramiv(progId, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(progId, MSG_BUFFER_SIZE, nullptr, msgBuffer);
            Log("Failed to create '" + std::to_string(progId) + "' shader program:" + msgBuffer, LogLevel::Error);
        }
    }
}

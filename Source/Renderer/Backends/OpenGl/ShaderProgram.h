#pragma once

namespace Silent::Renderer
{
    class ShaderProgram
    {
    private:
        // Fields

        std::string _name      = {};
        uint        _programId = 0;

    public:
        // Constructors

        ShaderProgram(const std::string& name);

        // Getters

        const std::string& GetName() const;
        uint               GetId() const;

        // Setters

        void SetBool(const std::string& uniName, bool val) const;
        void SetInt(const std::string& uniName, int val) const;
        void SetUint(const std::string& uniName, uint val) const;
        void SetFloat(const std::string& uniName, float val) const;
        void SetVector2(const std::string& uniName, const Vector2& val) const;
        void SetVector3(const std::string& uniName, const Vector3& val) const;
        void SetVector4(const std::string& uniName, const Vector4& val) const;
        void SetMatrix(const std::string& uniName, const Matrix& val) const;
        void SetIntArray(const std::string& uniName, const std::span<int>& val) const;
        void SetUintArray(const std::string& uniName, const std::span<uint>& val) const;
        void SetFloatArray(const std::string& uniName, const std::span<float>& val) const;
        void SetVector2Array(const std::string& uniName, const std::span<Vector2>& val) const;
        void SetVector3Array(const std::string& uniName, const std::span<Vector3>& val) const;
        void SetVector4Array(const std::string& uniName, const std::span<Vector4>& val) const;
        void SetMatrixArray(const std::string& uniName, const std::span<Matrix>& val) const;

        // Utilities

        void Activate() const;
        void Delete() const;

    private:
        // Helpers

        std::string GetFileContents(const std::string& filename) const;
        int         GetUniformLocation(const std::string& uniName) const;

        void LogShaderError(uint shaderId, const std::string& type) const;
        void LogProgramError(uint progId) const;
    };
}

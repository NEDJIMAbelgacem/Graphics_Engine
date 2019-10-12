#pragma once
#include "Core/Common.h"
#include "Uniforms/UniformTypes.h"

class UniformValue {
    std::string _label;
public:
    virtual UniformTypes GetType() = 0;
    virtual void UpdateShader(ShaderProgram& shader, std::string u_name) = 0;

    void SetLabel(std::string label) {
        this->_label = label;
    }

    std::string GetLabel() {
        return _label;
    }
};
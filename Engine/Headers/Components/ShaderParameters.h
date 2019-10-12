#pragma once
#include <Core/Common.h>
#include "Uniforms/UniformTypes.h"
#include "Uniforms/UniformValue.h"
#include "Core/ShaderProgram.h"

class ShaderParameters : public Component {
protected:
    std::unordered_map<std::string, UniformValue*> uniforms_map;
    // std::vector<std::string> update_buffer;
public:
    ShaderParameters() {
        this->id = (int)ComponentTypes::ShaderParameters;
    }

    ~ShaderParameters() { }

    void AddValue(std::string u_name, UniformValue& value) {
        this->uniforms_map[u_name] = &value;
        // update_buffer.push_back(u_name);
    }

    void EraseValue(std::string u_name) {
        this->uniforms_map.erase(u_name);
    }

    void UpdateShaders(ShaderProgram& shader) {
        for (auto& p : uniforms_map) p.second->UpdateShader(shader, p.first);
    }

    std::unordered_map<std::string, UniformValue*>& GetUniformsMap() {
        return uniforms_map;
    }
};

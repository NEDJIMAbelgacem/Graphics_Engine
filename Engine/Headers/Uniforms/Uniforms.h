#pragma once
#include "Core/Common.h"
#include "Uniforms/UniformTypes.h"
#include "Uniforms/UniformValue.h"
#include "Core/ShaderProgram.h"

struct Vec3Uniform : public UniformValue {
public:
    glm::vec3 val;
    UniformTypes GetType() { return UniformTypes::Float3; }

    void UpdateShader(ShaderProgram& shader, std::string u_name) {
        shader.FillUniformVec3(u_name, val);
    }
};

struct FloatUniform : public UniformValue {
public:
    float val;
    UniformTypes GetType() { return UniformTypes::Float1; }

    void UpdateShader(ShaderProgram& shader, std::string u_name) {
        shader.FillUniform1f(u_name, val);
    }
};
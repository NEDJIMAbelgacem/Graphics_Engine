#pragma once
#include "Core/Common.h"
#include "Core/ShaderProgram.h"

class DirectionalLight {
private:
    ShaderProgram* shader;
    glm::vec3 direction;
    glm::vec3 light_color;
public:
    DirectionalLight(ShaderProgram* shader, glm::vec3 direction, glm::vec3 light_color = glm::vec3(1.0f));
    ~DirectionalLight();
    void SetLightDirection(glm::vec3 direction);
    void SetLightColor(glm::vec3 color);
};
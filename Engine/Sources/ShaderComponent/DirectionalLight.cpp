#include "ShaderComponent/DirectionalLight.h"

DirectionalLight::DirectionalLight(ShaderProgram* shader, glm::vec3 direction, glm::vec3 light_color) {
    this->shader = shader;
    this->SetLightDirection(direction);
    this->SetLightColor(light_color);
}

DirectionalLight::~DirectionalLight() { }

void DirectionalLight::SetLightDirection(glm::vec3 direction) {
    if (this->direction != direction) {
        this->direction = direction;
        this->shader->FillUniformVec3("u_lightDir", direction);
    }
}

void DirectionalLight::SetLightColor(glm::vec3 color) {
    if (this->light_color != color) {
        this->light_color = color;
        this->shader->FillUniformVec3("u_lightDir", color);
    }
}
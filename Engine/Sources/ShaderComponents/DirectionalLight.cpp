#include "ShaderComponents/DirectionalLight.h"

void DirectionalLight::SetLightDirection(glm::vec3 direction) {
    if (this->direction != direction) {
        this->direction = direction;
        this->shader->FillUniformVec3("u_lightDir", direction);
    }
}

void DirectionalLight::SetLightColor(glm::vec3 color) {
    if (this->light_color != color) {
        this->light_color = color;
        this->shader->FillUniformVec3("u_lightColor", color);
    }
}
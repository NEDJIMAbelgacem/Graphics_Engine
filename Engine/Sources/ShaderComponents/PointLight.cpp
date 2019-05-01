#include "ShaderComponents/PointLight.h"

void PointLight::SetLightPosition(glm::vec3 position) {
    //if (this->light_pos != position) {
        this->light_pos = position;
        this->shader->FillUniformVec3("u_lightPos", position);
    //}
}

void PointLight::SetLightColor(glm::vec3 color) {
    if (this->light_color != color) {
        this->light_color = color;
        this->shader->FillUniformVec3("u_lightColor", color);
    }
}
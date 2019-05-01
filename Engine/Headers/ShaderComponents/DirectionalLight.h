#pragma once
#include "Core/Common.h"
#include "Core/ShaderProgram.h"
#include "ShaderComponents/Shaded.h"

class DirectionalLight : virtual public Shaded {
private:
    glm::vec3 direction;
    glm::vec3 light_color;
public:
    void SetLightDirection(glm::vec3 direction);
    void SetLightColor(glm::vec3 color);
};
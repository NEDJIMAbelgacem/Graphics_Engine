#pragma once
#include "Core/Common.h"
#include "Core/ShaderProgram.h"
#include "ShaderComponents/Shaded.h"

class PointLight : virtual public Shaded {
private:
    glm::vec3 light_pos;
    glm::vec3 light_color;
public:
    void SetLightPosition(glm::vec3 position);
    void SetLightColor(glm::vec3 color);
};
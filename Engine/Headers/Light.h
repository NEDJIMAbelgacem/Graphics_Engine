#pragma once
#include "Core/Common.h"

enum LightType {
    DIRECTIONAL_LIGHT = 0,
    POINT_LIGHT = 1
};

struct Light {
    LightType type;
    glm::vec3 color;
    glm::vec3 position;
};
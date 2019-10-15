#pragma once
#include "Core/Common.h"


// calculate triangle area using determinant method
// | v0.x v0.y 1 |
// | v1.x v1.y 1 |
// | v2.x v2.y 1 |
float TriangleArea(glm::vec2 v0, glm::vec2 v1, glm::vec2 v2) {
    return 0.5f * abs(v1.x * v2.y - v2.x * v1.y - v0.x * v2.y + v2.x * v0.y + v0.x * v1.y - v1.x * v0.y);
}

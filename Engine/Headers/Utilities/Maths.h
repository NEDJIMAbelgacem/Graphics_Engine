#pragma once
#include "Core/Common.h"


// calculate triangle area using determinant method
// | v0.x v0.y 1 |
// | v1.x v1.y 1 |
// | v2.x v2.y 1 |
float TriangleArea(glm::vec2 v0, glm::vec2 v1, glm::vec2 v2) {
    return 0.5f * abs(v1.x * v2.y - v2.x * v1.y - v0.x * v2.y + v2.x * v0.y + v0.x * v1.y - v1.x * v0.y);
}


// v0, v1, v2 shouldn't be in the same line (colinear ig)
glm::vec3 PlaneNormal(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2) {
    return glm::cross(v1 - v0, v2 - v0);
}

// v0, v1, v2 shouldn't be in the same line (colinear ig)
float Ray_Plane_Intersection(glm::vec3 origin, glm::vec3 ray, glm::vec3 v0, glm::vec3 v1, glm::vec3 v2) {
    glm::vec3 normal = PlaneNormal(v0, v1, v2);
    float a = normal.x, b = normal.y, c = normal.z;
    float d = -a * v0.x - b * v0.y - c * v0.z;
    float t = -(a * origin.x + b * origin.y + c * origin.z + d) / (a * ray.x + b * ray.y + c * ray.z);
    return t;
}

bool IsInsideTriangle(glm::vec3 p, glm::vec3 v0, glm::vec3 v1, glm::vec3 v2) {
    float area_v0v1v2 = 0.5f * glm::length(glm::cross(v1 - v0, v2 - v0));
    float alpha = 0.5f * glm::length(glm::cross(v1 - p, v2 - p)) / area_v0v1v2;
    float beta = 0.5f * glm::length(glm::cross(v2 - p, v0 - p)) / area_v0v1v2;
    float gamma = 1 - alpha - beta;
    return (alpha >= 0 && alpha <= 1 && beta >= 0 && beta <= 1 && gamma >= 0 && gamma <= 1);
}

bool GetBaryCentricCoordinates(glm::vec3 p, glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, float& alpha, float& beta, float& gamma) {
    glm::vec3 u = v1 - v0;
    glm::vec3 v = v2 - v0;
    glm::vec3 n = glm::cross(u, v);
    glm::vec3 w = p - v0;
    float a = glm::dot(n, n);
    gamma = glm::dot(glm::cross(u, w), n) / a;
    beta = glm::dot(glm::cross(w, v), n) / a;
    alpha = 1 - gamma - beta;
    return (alpha >= 0.0f && beta >= 0.0f && gamma >= 0.0f && alpha <= 1.0f && beta <= 1.0f && gamma <= 1.0f);
}
#pragma once
#include "Components/Component.h"
#include "Components/ComponentTypes.h"
#include "GameObject.h"

enum class LightingType {
    DirectionalLight,
    PointLight
};

struct Lighting : public Component {
    glm::vec3 position;
    glm::vec3 color = glm::vec3(0.0f);
    LightingType type;
    
    Lighting() {
        this->id = (int)ComponentTypes::Lighting;
    }
    
    Lighting(LightingType type, glm::vec3 position, glm::vec3 color)
        : type(type), position(position), color(color) {
        this->id = (int)ComponentTypes::Lighting;
    }
};
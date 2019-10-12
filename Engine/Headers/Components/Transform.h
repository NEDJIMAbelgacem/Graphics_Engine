#pragma once
#include "Components/Component.h"
#include "Components/ComponentTypes.h"
#include "GameObject.h"

class Transform : public Component {
public:
    glm::vec3 scale = glm::vec3(1.0f);
    glm::vec3 rotation = glm::vec3(0.0f);
    glm::vec3 position = glm::vec3(0.0f);
    
    Transform() { 
        this->id = (int)ComponentTypes::Transform;
    }
    Transform(glm::vec3& scale, glm::vec3& rotation, glm::vec3& position) 
        : scale(scale), rotation(rotation), position(position) {
        this->id = (int)ComponentTypes::Transform;
    }

public:
    void SetPosition(glm::vec3 position) { this->position = position; }
    void SetPosition(float x, float y, float z) { this->position = glm::vec3(x, y, z); }
    void SetRotation(glm::vec3 rotation) { this->rotation = rotation; }
    void SetRotation(float x, float y, float z) { this->rotation = glm::vec3(x, y, z); }
    void SetScale(glm::vec3 scale) { this->scale = scale; }
    void SetScale(float x, float y, float z) { this->scale = glm::vec3(x, y, z); }
};
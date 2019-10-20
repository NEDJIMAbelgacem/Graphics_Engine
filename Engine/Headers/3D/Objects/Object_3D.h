
#pragma once
#include "Core/Common.h"
#include "Core/ShaderProgram.h"
#include "Utilities/Maths.h"

namespace N3D {

struct Object_3D {
    Object_3D(glm::vec3 _position, glm::vec3 _rotation = glm::vec3(0.0f), glm::vec3 _scale = glm::vec3(1.0f)) 
    : position(_position), rotation(_rotation), scale(_scale) { }

    virtual void Render() { }
    virtual glm::mat4 GetModelMatrix() { return glm::identity<glm::mat4>(); }
    virtual glm::mat4 GetModelMatrixInverse() {
        glm::mat4 m = this->GetModelMatrix();
        return glm::inverse(m);
    }
    virtual void FillShader(ShaderProgram& prg) { }

    virtual bool DoIntersect(glm::vec3 origin, glm::vec3 ray) { return false; }
    virtual bool HandleMousePressedEvent(MouseButtonPressedEvent& e) { return false; }
    virtual bool HandleMouseMovedEvent(MouseMovedEvent& e) { return false; }

    virtual void SetPosition(glm::vec3 _position) { this->position = _position; }
    virtual glm::vec3 GetPosition() { return this->position; }

    void SetRotation(glm::vec3 rot) { this->rotation = rot; }
    glm::vec3 GetRotation() { return this->rotation; }

    void SetScale(glm::vec3 scale) { this->scale = scale; }
    glm::vec3 GetScale() { return this->scale; }

    virtual float GetX() { return this->position.x; }
    virtual float GetY() { return this->position.y; }
protected:
    bool is_grabbed = false;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
};

}
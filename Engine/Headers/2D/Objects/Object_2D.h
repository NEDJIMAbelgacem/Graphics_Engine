
#pragma once
#include "Core/Common.h"
#include "Core/ShaderProgram.h"
#include "Utilities/Maths.h"

namespace N3D {

struct Object_2D {
    Object_2D(glm::vec2 _position) : position(_position) { }

    virtual void Render() { }
    virtual glm::mat4 GetModelMatrix() { return glm::identity<glm::mat4>(); }
    virtual glm::mat4 GetModelMatrixInverse() {
        glm::mat4 m = this->GetModelMatrix();
        return glm::inverse(m);
    }
    virtual void FillShader(ShaderProgram& prg) { }
    virtual bool InBounds(float x, float y) {
        return false;
    }

    virtual void SetPosition(glm::vec2 _position) {
        this->position = _position;
    }
    virtual glm::vec2 GetPosition() { return this->position; }
    virtual float GetX() { return this->position.x; }
    virtual float GetY() { return this->position.y; }
protected:
    bool is_grabbed = false;
    glm::vec2 position;
    glm::vec2 grab_pos;
};

}
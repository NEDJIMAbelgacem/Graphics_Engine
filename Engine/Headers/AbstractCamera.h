
#pragma once
#include "Core/Common.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Core/ShaderProgram.h"

class AbstractCamera {
public:
    virtual void MoveForward(float distance) { }
    virtual void MoveBackward(float distance) { }
    virtual void MoveUp(float distance) { }
    virtual void MoveDown(float distance) { }
    virtual void MoveRight(float distance) { }
    virtual void MoveLeft(float distance) { }

    virtual void ZoomIn(float zoom_factor) { }
    virtual void ZoomOut(float zoom_factor) { }

    virtual glm::mat4 GetViewMatrix() { return glm::identity<glm::mat4>(); }
    virtual glm::mat4 GetViewMatrixInv() { return glm::inverse(this->GetViewMatrix()); }
    virtual glm::mat4 GetProjectionMatrix() { return glm::identity<glm::mat4>(); }
    virtual void FillShader(ShaderProgram& prg) {
        prg.FillUniformMat4f("u_view", GetViewMatrix());
        prg.FillUniformMat4f("u_proj", GetProjectionMatrix());
    }
};

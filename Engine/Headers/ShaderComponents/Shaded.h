#pragma once
#include "Core/Common.h"
#include "Core/ShaderProgram.h"

class Shaded {
protected:
    ShaderProgram* shader;
public:
    ShaderProgram* GetShader();
};
#pragma once
#include "Core/Common.h"
#include "Core/ShaderProgram.h"
#include "Core/VertexArray.h"
#include "Core/VertexBuffer.h"
#include "Core/IndexBuffer.h"

#include "GameObject.h"
#include "Components/Transform.h"
#include "Components/Surface.h"
#include "Components/Lighting.h"

#include "Geometry/Sphere.h"
#include "Light.h"

// TODO : handle modification in transform component so that it affects position and radius class members

class LightAnchor : public Sphere {
    Light* light = nullptr;
public: 
    LightAnchor(ShaderProgram* shader, Light* light, float radius) : Sphere(shader, light->position, radius) {
        this->light = light;
    }
    ~LightAnchor();

    void Update(float delta_time) override {
        this->transform.SetPosition(this->light->position);
    }
};
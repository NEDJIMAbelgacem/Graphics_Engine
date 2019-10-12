#pragma once
#include "Components/Component.h"
#include "Components/ComponentTypes.h"
#include "GameObject.h"

struct Surface : public Component {
    float reflectivity = 0.0f;
	float shine_damper = 1.0f;
	float diffuse_factor = 1.0f;

    Surface() { 
        this->id = (int)ComponentTypes::Surface;
    }
    Surface(float reflectivity, float shine_damper, float diffuse_factor)
        : reflectivity(reflectivity), shine_damper(shine_damper), diffuse_factor(diffuse_factor) {
        this->id = (int)ComponentTypes::Surface;
    }
};
#pragma once
#include "Core/Common.h"
#include "ShaderComponents/Shaded.h"

class SurfaceParameters : virtual public Shaded {
protected:
    float reflectivity;
	float shine_damper;
	float diffuse_factor;
public:
    void SetSurfaceParameters(float reflectivity, float shine_damper, float diffuse_factor);
    void SetReflectivity(float reflectivity) { this->reflectivity = reflectivity; }
    void SetShineDamper(float shine_damper) { this->shine_damper = shine_damper; }
    void SetDiffuseFator(float diffuse_factor) { this->diffuse_factor = diffuse_factor; }
};
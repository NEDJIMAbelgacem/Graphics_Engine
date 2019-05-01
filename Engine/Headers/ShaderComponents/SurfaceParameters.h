#pragma once
#include "Core/Common.h"
#include "ShaderComponents/Shaded.h"

class SurfaceParameters : virtual public Shaded {
protected:
    float reflectivity;
	float shineDamper;
	float diffuseFactor;
public:
    void SetSurfaceParameters(float reflectivity, float shineDamper, float diffuseFactor);
};
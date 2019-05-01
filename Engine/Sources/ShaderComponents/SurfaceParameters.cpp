#include "ShaderComponents/SurfaceParameters.h"

void SurfaceParameters::SetSurfaceParameters(float reflectivity, float shineDamper, float diffuseFactor) {
	if (this->reflectivity != reflectivity) {
		this->reflectivity = reflectivity;
		shader->FillUniform1f("u_material.reflectivity", reflectivity);
	}
	
	if (this->shineDamper != shineDamper) {
		this->shineDamper = shineDamper;
		shader->FillUniform1f("u_material.shineDamper", shineDamper);
	}

	if (this->diffuseFactor != diffuseFactor) {
		this->diffuseFactor = diffuseFactor;
		shader->FillUniform1f("u_material.diffuseFactor", diffuseFactor);
	}
}
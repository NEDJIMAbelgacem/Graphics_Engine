#include "ShaderComponents/SurfaceParameters.h"

void SurfaceParameters::SetSurfaceParameters(float reflectivity, float shineDamper, float diffuseFactor) {
	if (this->reflectivity != reflectivity) {
		this->reflectivity = reflectivity;
		shader->FillUniform1f("u_material.reflectivity", reflectivity);
	}
	
	if (this->shine_damper != shine_damper) {
		this->shine_damper = shine_damper;
		shader->FillUniform1f("u_material.shineDamper", shine_damper);
	}

	if (this->diffuse_factor != diffuse_factor) {
		this->diffuse_factor = diffuse_factor;
		shader->FillUniform1f("u_material.diffuseFactor", diffuse_factor);
	}
}
#shader vertex
#version 430 core

layout(location = 0) in vec3 verticeCoords;
layout(location = 1) in vec2 verticeTexture;
layout(location = 2) in vec3 verticeNormal;
layout(location = 3) in vec3 offset;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;
uniform int u_isBatchRender;
uniform int u_isClipEnabled;
uniform vec4 u_clipPlane;

out vec3 v_normal;
out vec3 v_fragPos;
out vec2 v_texCoords;

void main() {
	vec4 worldPos = u_model * vec4(verticeCoords, 1.0f);
	if (u_isBatchRender != 0) 
		worldPos = vec4(worldPos.x + offset.x, worldPos.y + offset.y, worldPos.z + offset.z, 1.0f);
	if (u_isClipEnabled != 0)
		gl_ClipDistance[0] = dot(worldPos, u_clipPlane);
	
	gl_Position = u_proj * u_view * worldPos;
	v_fragPos = worldPos.xyz;
	v_normal = (u_model * vec4(verticeNormal, 1.0f)).xyz;
	v_texCoords = verticeTexture;
}

#shader fragment
#version 430 core

struct Material {
	sampler2D ambientMaps[7];
	sampler2D diffuseMaps[7];
	sampler2D specularMaps[7];
	sampler2D heightMaps[7];
	sampler2D materialTexture;
	float reflectivity;
	float shineDamper;
	float diffuseFactor;
};

out vec4 fragColor;

in vec2 v_texCoords;
in vec3 v_fragPos;
in vec3 v_normal;
uniform Material u_material;
uniform vec3 u_cameraPos;

uniform vec3 u_lightDir;
uniform vec3 u_lightColor;

void main() {
	vec3 lightDir = normalize(u_lightDir);
	vec3 toCamera = normalize(u_cameraPos - v_fragPos);
	vec3 norm = normalize(v_normal);
	vec3 reflVect = reflect(-lightDir, norm);

    vec4 material_texel = texture(u_material.materialTexture, v_texCoords);
	// ambient light
	vec4 ambientL =u_lightColor * material_texel;
	// diffuse light
	vec4 diffuseL = 1.0f * max(dot(norm, lightDir), 0.0f) * material_texel;//texture(u_material.diffuseMaps[0], v_texCoords);
	// specular light
	vec4 specularL = pow(max(dot(reflVect, toCamera), 0.0f), 32) * material_texel;//texture(u_material.specularMaps[0], v_texCoords);
	
	fragColor = material_texel;//(0.1 * ambientL + 0.9 * (diffuseL + specularL)) * vec4(u_lightColor, 1.0f);
    //fragColor = vec4(1.0f);
}
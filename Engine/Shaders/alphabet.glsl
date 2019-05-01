#shader vertex
#version 430 core

layout(location = 0) in vec3 verticeCoords;
layout(location = 1) in vec2 verticeTexture;
layout(location = 2) in vec3 verticeNormal;
layout(location = 3) in vec3 offset;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;
uniform int u_isClipEnabled;
uniform vec4 u_clipPlane;

out vec3 v_normal;
out vec3 v_fragPos;
out vec2 v_texCoords;

void main() {
	vec4 worldPos = u_model * vec4(verticeCoords, 1.0f);
	if (u_isClipEnabled != 0)
		gl_ClipDistance[0] = dot(worldPos, u_clipPlane);
	gl_Position = u_proj * u_view * worldPos;
	v_fragPos = worldPos.xyz;
	v_normal = normalize(verticeNormal);//normalize((u_model * vec4(verticeNormal, 1.0f)).xyz);
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
uniform vec3 u_lightDir;
uniform vec3 u_lightColor;

uniform vec3 u_cameraPos;

void main() {
	vec3 lightDir = normalize(u_lightDir);
	vec3 toCamera = normalize(u_cameraPos - v_fragPos);
	vec3 reflVect = reflect(-lightDir, v_normal);
	// ambient light
	vec4 ambientL = vec4(0.5 * v_normal + 0.5, 0);
	// diffuse light
	vec4 diffuseL = 0.0f * max(dot(v_normal, lightDir), 0.0f) * vec4(1.0f, 1.0f, 1.0f, 1.0f);
	// specular light
	vec4 specularL = 0.0f * pow(max(dot(reflVect, toCamera), 0.0f), u_material.shineDamper) * vec4(1.0f, 1.0f, 1.0f, 1.0f);

	fragColor = vec4((ambientL + specularL + specularL).xyz * u_lightColor, 1.0f);
}
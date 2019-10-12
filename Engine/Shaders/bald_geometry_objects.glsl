#shader vertex
#version 430 core

layout(location = 0) in vec3 verticeCoords;
layout(location = 1) in vec2 verticeTexture;
layout(location = 2) in vec3 verticeNormal;
layout(location = 3) in vec3 offset;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;
uniform float u_near;
uniform float u_far;
uniform int u_isBatchRender;
uniform int u_isClipEnabled;
uniform vec4 u_clipPlane;

out vec3 v_normal;
out vec3 v_fragPos;
out vec2 v_texCoords;

void main() {
	float v = u_near + u_far;
	vec4 worldPos = u_model * vec4(verticeCoords, 1.0f);
	if (u_isBatchRender != 0) 
		worldPos = vec4(worldPos.x + offset.x, worldPos.y + offset.y, worldPos.z + offset.z, 1.0f);
	if (u_isClipEnabled != 0)
		gl_ClipDistance[0] = dot(worldPos, u_clipPlane);
	
	gl_Position = u_proj * u_view * worldPos;
	v_fragPos = worldPos.xyz;
	v_normal = (u_model * vec4(verticeNormal, 0.0f)).xyz;
	v_texCoords = verticeTexture;
}

#shader fragment
#version 430 core
#define MAX_LIGHTS_COUNT 10
#define DIRECTIONAL_LIGHT 0
#define POINT_LIGHT 1

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

struct Light {
    int type;
    vec3 color;
    vec3 position;
};

out vec4 fragColor;

in vec2 v_texCoords;
in vec3 v_fragPos;
in vec3 v_normal;
uniform Material u_material;
uniform vec3 u_cameraPos;

uniform int u_lights_count;
uniform Light u_lights[MAX_LIGHTS_COUNT];

void main() {
    vec4 material_texel = texture(u_material.materialTexture, v_texCoords);
    material_texel = vec4(1.0f);

	// ambient light
	vec4 ambientL = 0.05 * material_texel;
    vec4 diffuseL = vec4(0.0f);
    vec4 specularL = vec4(0.0f);
    for (int i = 0; i < u_lights_count; ++i) {
        vec3 lightDir = normalize(v_fragPos - u_lights[i].position);
        vec3 toCamera = normalize(u_cameraPos - v_fragPos);
        vec3 norm = normalize(v_normal);
        vec3 reflVect = reflect(lightDir, norm);
        diffuseL += max(dot(-lightDir, norm), 0.0f) * vec4(u_lights[i].color, 1.0f) * material_texel;
        specularL += 0.05 * pow(max(dot(reflVect, toCamera), 0.0f), 1) * vec4(u_lights[i].color, 1.0f) * material_texel;
    }
	// diffuse light
	// vec4 diffuseL = max(dot(-lightDir, norm), 0.0f) * material_texel;
	// specular light
	// vec4 specularL = 0.05 * pow(max(dot(reflVect, toCamera), 0.0f), 1) * material_texel;//texture(u_material.specularMaps[0], v_texCoords);
	
	fragColor = (ambientL + diffuseL + specularL);
}
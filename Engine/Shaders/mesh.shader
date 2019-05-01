#shader vertex
#version 430

layout(location = 0) in vec3 verticeCoords;
layout(location = 1) in vec2 verticeTexture;
layout(location = 2) in vec3 verticeNormal;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;
//uniform int u_isClipEnabled;
//uniform vec4 u_clipPlane;

out vec3 v_normal;
out vec3 v_fragPos;
out vec2 v_texCoords;

void main() {
	vec4 worldPos = u_model * vec4(verticeCoords, 1.0f);
	//if (u_isClipEnabled != 0)
	//	gl_ClipDistance[0] = dot(worldPos, u_clipPlane);
	gl_Position = u_proj * u_view * worldPos;
	//v_fragPos = worldPos.xyz;
	//v_normal = (u_model * vec4(verticeNormal, 1.0f)).xyz;
	//v_texCoords = verticeTexture;
}

#shader fragment
#version 430

struct Material {
	vec3 ambientLColor;
	vec3 diffuseLColor;
	vec3 specularLColor;
	vec3 materialColor;
	float reflectivity;
	float shineDamper;
	float diffuseFactor;
};

out vec4 fragColor;

in vec2 v_texCoords;
in vec3 v_fragPos;
in vec3 v_normal;
uniform Material u_material;
uniform vec3 u_lightPos;
uniform vec3 u_cameraPos;

uniform sampler2D u_tex;
uniform vec3 u_light_color;

void main() {
	//vec3 lightDir = normalize(u_lightPos - v_fragPos);
	//vec3 toCamera = normalize(u_cameraPos - v_fragPos);
	//vec3 norm = normalize(v_normal);
	//vec3 reflVect = reflect(-lightDir, norm);
	// ambient light
	//vec3 ambientL = u_material.ambientLColor;
	// diffuse light
	//vec3 diffuseL = u_material.diffuseFactor * max(dot(norm, lightDir), 0.0f) * u_material.diffuseLColor;
	// specular light
	//vec3 specularL = u_material.reflectivity * pow(max(dot(reflVect, toCamera), 0.0f), u_material.shineDamper) * u_material.specularLColor;
	fragColor = vec4(1.0f);//vec4((ambientL + diffuseL + specularL) * u_material.materialColor * u_light_color, 1.0f) * texture(u_tex, v_texCoords);
}
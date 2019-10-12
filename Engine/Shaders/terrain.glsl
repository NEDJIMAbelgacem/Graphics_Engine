#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoords;

out vec2 v_texCoords;
out vec3 v_normal;
out vec3 v_fragPos;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

uniform int u_tiles_per_side;
uniform int u_vertices_per_tile;
uniform float u_tile_size;

uniform int u_isClipEnabled;
uniform vec4 u_clipPlane;

uniform sampler2D u_heightMap;
uniform float u_height_amplitude;

float get_height(in vec2 pos) {
	float h = texture(u_heightMap, pos).r - 0.5f;
	return h * u_height_amplitude;
}

vec3 get_normal(in vec2 heightMap_texCoords) {
	float worldCoords_step = u_tile_size / u_vertices_per_tile;
	float heightMapCoords_step = 1.0f / (u_vertices_per_tile * u_tiles_per_side * u_tile_size);
	vec2 pos = heightMap_texCoords * u_vertices_per_tile * u_tiles_per_side * u_tile_size;
	float current_height = get_height(heightMap_texCoords);
	vec3 v1, v2;
	float left_height, down_height, up_height, right_height;
	switch (gl_VertexID) {
	case 0:
		left_height = get_height(vec2(min(heightMap_texCoords.x + heightMapCoords_step, 1.0f), heightMap_texCoords.y));
		down_height = get_height(vec2(heightMap_texCoords.x, min(heightMap_texCoords.y + heightMapCoords_step, 1.0f)));
		v1 = vec3(worldCoords_step, left_height - current_height, 0);
		v2 = vec3(0, down_height - current_height, worldCoords_step);
		break;
	case 1:
		left_height = get_height(vec2(min(heightMap_texCoords.x + heightMapCoords_step, 1.0f), heightMap_texCoords.y));
		up_height = get_height(vec2(heightMap_texCoords.x, max(heightMap_texCoords.y - heightMapCoords_step, 0.0f)));
		v1 = vec3(worldCoords_step, left_height - current_height, 0);
		v2 = vec3(0, up_height - current_height, -worldCoords_step);
		break;
	case 2:
		right_height = get_height(vec2(max(heightMap_texCoords.x - heightMapCoords_step, 0.0f), heightMap_texCoords.y));
		up_height = get_height(vec2(heightMap_texCoords.x, max(heightMap_texCoords.y - heightMapCoords_step, 0.0f)));
		v1 = vec3(-worldCoords_step, right_height - current_height, 0);
		v2 = vec3(0, up_height - current_height, -worldCoords_step);
		break;
	case 3:
		right_height = get_height(vec2(max(heightMap_texCoords.x - heightMapCoords_step, 0.0f), heightMap_texCoords.y));
		down_height = get_height(vec2(heightMap_texCoords.x, min(heightMap_texCoords.y + heightMapCoords_step, 1.0f)));
		v1 = vec3(-worldCoords_step, right_height - current_height, 0);
		v2 = vec3(0, down_height - current_height, worldCoords_step);
		break;
	}
	return cross(v1, v2);
}

// a tile is textured using the whole texture
// a block is used to give heights to vertces
void main() {
    float terrain_size = u_tiles_per_side * u_tile_size;
	float sub_tile_size = u_tile_size / float(u_vertices_per_tile);
    // block represents vertice to vertice square patrition
	int blocks_count = u_vertices_per_tile * u_vertices_per_tile;

	int block_instance = gl_InstanceID % blocks_count;
	int tile_instance = gl_InstanceID / blocks_count;
    
    int block_x_index = block_instance / u_vertices_per_tile;
	int block_y_index = block_instance % u_vertices_per_tile ;
	int tile_x_index = tile_instance / u_tiles_per_side;
	int tile_y_index = tile_instance % u_tiles_per_side;

    float vertice_x_offset = texCoords.x * sub_tile_size;
    float vertice_z_offset = texCoords.y * sub_tile_size;
    // block_coords is mapped to [0, u_tile_size]
	float block_x_offset = block_x_index * sub_tile_size;
	float block_z_offset = block_y_index * sub_tile_size;
    // tile_coords is mapped to [0, terrain_size]
    float tile_x_offset = float(tile_x_index) * u_tile_size;
	float tile_z_offset = float(tile_y_index) * u_tile_size;

	//float tex_x_offset = float(tile_x_index) / float(u_vertices_per_tile);
	//float tex_y_offset = float(tile_y_index) / float(u_vertices_per_tile);
	//float tex_x = texCoords.x / float(u_vertices_per_tile) + tex_x_offset;
	//float tex_y = texCoords.y / float(u_vertices_per_tile) + tex_y_offset;
    // texture coordinates in tile space
	float tex_x = (vertice_x_offset + block_x_offset) / u_tile_size;
    float tex_y = (vertice_z_offset + block_z_offset) / u_tile_size;
    v_texCoords = vec2(tex_y, tex_x);

    float x = vertice_x_offset + tile_x_offset + block_x_offset;
    float z = vertice_z_offset + sub_tile_size + tile_z_offset + block_z_offset;
	vec2 heightMap_texCoords = vec2(x, z) / terrain_size;

	//float x = position.x * float(sub_tile_size) + internal_x_offset + block_x_offset;
	float y = get_height(heightMap_texCoords);
	//float z = position.z * float(sub_tile_size) + internal_z_offset + block_z_offset;

	vec4 worldPos = u_model * vec4(x, y, z, 1.0f);
	gl_Position = u_proj * u_view * worldPos;
	if (u_isClipEnabled != 0)
		gl_ClipDistance[0] = dot(worldPos, u_clipPlane);
	v_fragPos = (vec4(x, y, z, 1.0f)).xyz;
	
	v_normal = get_normal(heightMap_texCoords);
}

#shader fragment
#version 330 core

struct Material {
	sampler2D diffuseMaps[8];
	sampler2D specularMap;
	sampler2D heightMap;
	sampler2D materialTexture;
	float reflectivity;
	float shineDamper;
	float diffuseFactor;
};

out vec4 fragColor;

in vec3 v_fragPos;
in vec3 v_normal;
in vec2 v_texCoords;

uniform Material u_material;
uniform vec3 u_lightPos;
uniform vec3 u_cameraPos;
uniform float u_height_amplitude;

uniform vec3 u_lightColor;

void main() {
	vec3 lightDir = normalize(u_lightPos - v_fragPos);
	vec3 toCamera = normalize(u_cameraPos - v_fragPos);
	vec3 norm = normalize(v_normal);
	vec3 reflVect = reflect(-lightDir, norm);
	
	// diffuse light
	float f = (v_fragPos.y / u_height_amplitude + 1.0f) * 0.75;
	// float f = 1.0;
	vec4 diffuseL = u_material.diffuseFactor * pow(dot(norm, lightDir), 1) * mix(texture(u_material.diffuseMaps[0], v_texCoords), texture(u_material.diffuseMaps[0], v_texCoords), smoothstep(0.4, 0.6f, f));
	// specular light
	vec4 specularL = pow(max(dot(reflVect, toCamera), 0.0f), u_material.shineDamper) * texture(u_material.diffuseMaps[0], v_texCoords);

	fragColor = mix(texture(u_material.diffuseMaps[0], v_texCoords), texture(u_material.diffuseMaps[1], v_texCoords), f);//smoothstep(0.4, 0.6f, f));//vec4((diffuseL + specularL).xyz * u_lightColor, 1.0f);
}
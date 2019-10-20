#shader vertex
#version 430
#define MAX_ARRAY_SIZE 1000

layout(location = 0) in vec3 position;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

layout (std140, binding = 2) uniform u_colors
{
    ivec4 color_index[MAX_ARRAY_SIZE];
};

layout (std140, binding = 3) uniform u_selected_triangles
{
    ivec4 selected[MAX_ARRAY_SIZE];
};

out vec2 v_frag_coords;
out vec3 v_color;
out float is_selected;

vec3 hsv2rgb(vec3 c)
{
	vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
	vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
	return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main() {
    v_frag_coords = position.xy;
    gl_Position = u_proj * u_view * u_model * vec4(position, 1.0f);
    int triangle_index = gl_VertexID / 3;
    // if (gl_VertexID / 3 == u_selected_triangle) is_selected = 1.0f;
    // else is_selected = 0.0f;
    is_selected = 0.0f;
    for (int i = 1; i <= selected[0].x; ++i) {
        int x = i / 4;
        int y = i % 4;
        if (triangle_index == selected[x][y]) {
            is_selected = 1.0f;
            break;
        }
    }
    

    int color_id = color_index[gl_VertexID / 3 / 4][gl_VertexID / 3 % 4] % 3;

    if (color_id == 0) v_color = vec3(1.0f, 0.0f, 0.0f);
    else if (color_id == 1) v_color = vec3(0.0f, 1.0f, 0.0f);
    else if (color_id == 2) v_color = vec3(0.0f, 0.0f, 1.0f);
    else if (color_id == 3) v_color = vec3(1.0f, 0.0f, 1.0f);
    else if (color_id == 4) v_color = vec3(0.0f, 0.0f, 0.0f);
    else v_color = vec3(1.0f, 1.0f, 1.0f);
}

#shader fragment
#version 430

uniform int u_texture_is_used;
uniform sampler2D u_texture;
uniform vec3 u_color;

in float is_selected;
in vec3 v_color;
in vec2 v_frag_coords;
out vec4 fragColor;

void main() {
	// u_color;
    // if (is_selected > 0) fragColor = vec4(1.0f, 0.0f, 1.0f, 1.0f);
	// else 
    fragColor = vec4(0.5f * v_color + vec3(0.3 * is_selected), 1.0f);
}
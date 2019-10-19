#shader vertex
#version 430

layout(location = 0) in vec3 position;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

layout (std140, binding = 2) uniform u_colors
{
    int color_index[1000];
};  

out vec2 v_frag_coords;
out vec3 v_color;

void main() {
    v_frag_coords = position.xy;
    gl_Position = u_proj * u_view * u_model * vec4(position, 1.0f);
    int color_id = color_index[gl_VertexID / 9] / 4 % 4;
    if (color_id == 0) v_color = vec3(1.0f, 0.0f, 0.0f);
    else if (color_id == 1) v_color = vec3(0.0f, 1.0f, 0.0f);
    else if (color_id == 2) v_color = vec3(0.0f, 0.0f, 1.0f);
    else v_color = vec3(0.0f, 1.0f, 1.0f);
}

#shader fragment
#version 430

uniform int u_texture_is_used;
uniform sampler2D u_texture;
uniform vec3 u_color;

in vec3 v_color;
in vec2 v_frag_coords;
out vec4 fragColor;

void main() {
	// u_color;
	fragColor = vec4(v_color, 1.0f);
}
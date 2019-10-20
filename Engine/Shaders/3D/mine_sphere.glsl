#shader vertex
#version 430

layout(location = 0) in vec3 position;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

layout (std140, binding = 2) uniform u_colors
{
    ivec4 color_index[1000];
    // int color0;
    // int color1;
    // int color2;
    // int color3;
    // int color4;
    // int color5;
};

out vec2 v_frag_coords;
out vec3 v_color;


vec3 hsv2rgb(vec3 c)
{
	vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
	vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
	return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main() {
    v_frag_coords = position.xy;
    gl_Position = u_proj * u_view * u_model * vec4(position, 1.0f);

    int color_id = color_index[gl_VertexID / 3 / 4][gl_VertexID / 3 % 4] % 3;
    // float id = color_id;
    // int color_id = gl_VertexID / 3;

    // if (color_id == 0) color_id = color0;
    // else if (color_id == 1) color_id = color1;
    // else if (color_id == 2) color_id = color2;
    // else if (color_id == 3) color_id = color3;
    // else if (color_id == 4) color_id = color4;
    // else color_id = color5;

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

in vec3 v_color;
in vec2 v_frag_coords;
out vec4 fragColor;

void main() {
	// u_color;
	fragColor = vec4(v_color, 1.0f);
}
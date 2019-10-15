#shader vertex
#version 430

layout(location = 0) in vec3 position;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

out vec2 v_frag_coords;

void main() {
	v_frag_coords = position.xy;
	gl_Position = u_proj * u_view * u_model * vec4(position, 1.0f);
}

#shader fragment
#version 430

uniform int u_texture_is_used;
uniform sampler2D u_texture;
uniform vec3 u_color;

in vec2 v_frag_coords;
out vec4 fragColor;

void main() {
	u_color;
	if (u_texture_is_used == 1) {
		// fragColor = vec4(v_frag_coords, 0.0f, 1.0f);
		fragColor = texture(u_texture, v_frag_coords);
	} else {
		fragColor = vec4(u_color, 1.0f);
	}
}
#shader vertex
#version 430

layout(location = 0) in vec3 position;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

uniform int u_texture_is_used;
out vec2 v_frag_coords;

void main() {
	if (u_texture_is_used == 0) {
		v_frag_coords = position.xy;
		gl_Position = u_proj * u_view * u_model * vec4(position, 1.0f);
	} else {
		vec4 p = vec4(position, 1.0f);

		vec3 d = normalize(vec3(p));
		float u = 0.5f + 0.5f * atan(d.z, -d.x) / 3.14159f;
		float v = 0.5f - asin(d.y) / 3.14159f;
		v_frag_coords = vec2(u, v);

		gl_Position = u_proj * u_view * u_model * p;
	}
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
		fragColor = vec4(texture(u_texture, clamp(v_frag_coords, 0.0f, 1.0f)).rgb, 1.0f);
	} else {
		fragColor = vec4(u_color, 1.0f);
	}
}
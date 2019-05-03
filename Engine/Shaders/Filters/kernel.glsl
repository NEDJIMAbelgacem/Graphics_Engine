#shader vertex
#version 330

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texCoords;

out vec2 v_texCoords;

void main() {
	v_texCoords = texCoords;
	gl_Position = vec4(position, 0.0f, 1.0f);
}

#shader fragment
#version 330

out vec4 fragColor;

in vec2 v_texCoords;
uniform sampler2D screenTexture;
uniform float offset;

void main() {
	vec2 offsets[9] = vec2[](
		vec2(-offset, offset),
		vec2(0.0f, offset),
		vec2(offset, offset),
		vec2(-offset, 0.0f),
		vec2(0.0f, 0.0f),
		vec2(offset, 0.0f),
		vec2(-offset, -offset),
		vec2(0.0f, -offset),
		vec2(offset, -offset)
	);

	float kernel[9] = float[9](
		-1, -1, -1,
		-1,  9, -1,
		-1, -1, -1
	);

	vec3 sampleTex[9];
	vec3 color = vec3(0.0f);
	for (int i = 0; i < 9; ++i) 
		color +=  kernel[i] * vec3(texture(screenTexture, v_texCoords.st + offsets[i]));

	fragColor = vec4(color, 1.0f);
}
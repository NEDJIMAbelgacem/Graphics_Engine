#shader vertex
#version 430

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texCoords;

out vec2 v_texCoords;

void main() {
	v_texCoords = texCoords;
	gl_Position = vec4(position, 0.0f, 1.0f);
}

#shader fragment
#version 430

out vec4 fragColor;

in vec2 v_texCoords;
uniform sampler2D screenTexture;

void main() {
	fragColor = texture(screenTexture, v_texCoords);
}
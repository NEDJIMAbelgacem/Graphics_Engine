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

void main() {
	fragColor = texture(screenTexture, v_texCoords);
	float average = 0.2126 * fragColor.r + 0.7152 * fragColor.g + 0.0722 * fragColor.b;
	fragColor = vec4(average, average, average, 1.0);
}
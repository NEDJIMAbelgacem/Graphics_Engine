#shader vertex
#version 430

layout (location = 0) in vec3 vertex; // vec3 pos
layout (location = 1) in vec2 tex; // vec2 tex
out vec2 TexCoords;

uniform mat4 u_proj;
uniform mat4 u_view;
uniform mat4 u_model;

void main() {
    gl_Position = u_proj * u_view * u_model * vec4(vertex, 1.0);
    TexCoords = tex.xy;
} 

#shader fragment
#version 430

in vec2 TexCoords;
out vec4 color;

uniform sampler2D u_text;
uniform vec3 u_textColor;

void main() {
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(u_text, TexCoords).r);
    color = vec4(u_textColor, 1.0) * sampled;
} 
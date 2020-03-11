#shader vertex
#version 430

layout (location = 0) in vec2 vertex; // pos

uniform mat4 u_proj;

void main() {
    gl_Position = u_proj * vec4(vertex.xy, 0.0, 1.0);
}

#shader fragment
#version 430

out vec4 color;

uniform vec3 u_color;

void main()
{    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(u_text, TexCoords).r);
    color = vec4(u_textColor, 1.0) * sampled;
}
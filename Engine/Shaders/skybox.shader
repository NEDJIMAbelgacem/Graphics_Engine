#shader vertex
#version 430 core

layout(location = 0) in vec2 pos;

out vec3 v_ray;
out vec2 v_pos;

uniform mat4 u_invprojview;
uniform float u_near;
uniform float u_far;

void main() {
	gl_Position = vec4(pos, 0.0, 1.0);
	v_pos = pos;
	v_ray = -(u_invprojview * vec4(pos * (u_far - u_near), u_far + u_near, u_far - u_near)).xyz;
}

#shader fragment
#version 430 core

uniform float u_near;
uniform float u_far;

in vec2 v_pos;
in vec3 v_ray;
out vec4 fragColor;

uniform samplerCube u_skybox;

float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0;
    return (2.0 * u_near * u_far) / (u_far + u_near - z * (u_far - u_near));	
}

void main() {
    //if (LinearizeDepth(gl_FragCoord.z) >= 0) discard;
    //float depth = LinearizeDepth(gl_FragCoord.z) / u_far;
	fragColor = vec4(texture(u_skybox, v_ray).rgb, 1.0f);
    //fragColor = vec4(vec3(depth), 1.0f);
}
#shader vertex
#version 430 core

layout(location = 0) in vec3 pos;

out vec3 v_ray;
out vec3 v_pos;

uniform mat4 u_view;
uniform mat4 u_proj;
// uniform float u_near;
// uniform float u_far;
// uniform vec3 u_cameraPos;


void main() {
    // u_cameraPos;
    // float tan_fov = tan(radians(90.0f) / 2.0f);
    // float aspect_ratio = 1200.0f / 900.0f;
    // float x = pos.x * tan_fov * aspect_ratio;
    // float y = pos.y * tan_fov;
	// v_ray = (inverse(u_proj * u_view) * vec4(pos * (u_far - u_near), u_far + u_near, u_far - u_near)).xyz;
    v_pos = vec3(pos.xy, 0.0f);//(inverse(u_proj * u_view) * vec4(pos.xy, 1.0f, 1.0f)).xyz;
    // vec3 vertices_world_pos = (inverse(u_proj * u_view) * vec4(pos, 0.0f, 1.0f)).xyz;
    // v_ray = vertices_world_pos - camera_world_pos;

    gl_Position = vec4(pos.xy, 0.0f, 1.0f);
}

#shader fragment
#version 430 core

uniform mat4 u_proj;
uniform mat4 u_view;
uniform float u_far;
uniform vec3 u_cameraPos;


in vec3 v_pos;
in vec3 v_ray;
out vec4 fragColor;

uniform samplerCube u_skybox;
uniform int u_is_hdr;
const float gamma = 2.2;

// float LinearizeDepth(float depth) 
// {
//     float z = depth * 2.0 - 1.0;
//     return (2.0 * u_near * u_far) / (u_far + u_near - z * (u_far - u_near));	
// }

vec3 gamma_correct(vec3 color) {
    color = color / (color + vec3(1.0f));
    color = pow(color, vec3(1.0 / gamma));
    return color;
}

void main() {
    u_cameraPos;
    u_skybox;
    u_proj;
    vec4 pixel_pos = inverse(u_proj) * vec4(v_pos.xy, 0.0f, 1.0f);
    pixel_pos.w = 1.0f;
    pixel_pos = inverse(u_view) * pixel_pos;
    vec3 ray = normalize(pixel_pos.xyz - u_cameraPos);
    //if (LinearizeDepth(gl_FragCoord.z) >= 0) discard;
    // float depth = LinearizeDepth(gl_FragCoord.z) / u_far;
    vec3 sky_color = texture(u_skybox, ray).rgb;
    if (u_is_hdr == 1) fragColor = vec4(gamma_correct(sky_color), 1.0f);
    else fragColor = vec4(sky_color, 1.0f);
    // discard;
    // fragColor = u_proj * vec4(v_pos, 1.0f);//vec4(1.0f, 0.0f, 0.0f, 1.0f);
    
    // fragColor = vec4(ray, 1.0f);
}
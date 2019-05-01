#shader vertex
#version 430 core

layout(location = 0) in vec2 pos;

uniform mat4 u_proj_matrix;
uniform mat4 u_view_matrix;
uniform float u_near;
uniform float u_far;

uniform vec3 cameraPos;

out vec3 v_origin;
out vec3 v_ray;

void main() {
	gl_Position = vec4(pos, 0.0, 1.0);
	mat4 invprojview = inverse(u_proj_matrix * u_view_matrix);
	v_origin = (invprojview * (vec4(pos, -1.0, 1.0) * u_near)).xyz;
	v_ray = -(invprojview * vec4(pos * (u_far - u_near), u_far + u_near, u_far - u_near)).xyz;
}

#shader fragment
#version 430 core

in vec3 v_origin;
in vec3 v_ray;

out vec4 fragColor;

uniform float u_scale;
uniform float u_MinRad2;
uniform int u_iterations;
uniform float u_bailout;

uniform float u_raymarching_max_distance;
uniform float u_raymarching_min_distance;
uniform int u_raymarching_max_steps;

const float max_distance = u_raymarching_max_distance;
const float min_distance = u_raymarching_min_distance;
const int max_steps = u_raymarching_max_steps;

float Scale = u_scale;
float MinRad2 = u_MinRad2;
int Iterations = u_iterations;
float Bailout = u_bailout;

vec4 scale = vec4(Scale, Scale, Scale, abs(Scale)) / MinRad2;
float fractal_signed_distance(vec3 pos) {
	vec4 p = vec4(pos, 1);
	vec4 p0 = p;  // p.w is the distance estimate
	int i;
	for (i = 0; i < Iterations; i++) {
		p.xyz = clamp(p.xyz, -1.0, 1.0) * 2.0f - p.xyz; 
		float r2 = dot(p.xyz, p.xyz);
		p *= clamp(max(MinRad2 / r2, MinRad2), 0.0f, 1.0f);  // dp3,div,max.sat,mul
		p = p * scale + p0;
		if (r2 > Bailout) break;
	}
	if (i == Iterations) discard;
	return ((length(p.xyz) - abs(Scale - 1.0)) / p.w - pow(abs(Scale), float(1 - Iterations)));
}

float fractal_color(vec3 pos) {
	vec4 p = vec4(pos, 1);
	vec4 p0 = p;  // p.w is the distance estimate
	int i = 0;
	for (i = 0; i < Iterations; i++) {
		p.xyz = clamp(p.xyz, -1.0, 1.0) * 2.0 - p.xyz;
		float r2 = dot(p.xyz, p.xyz);
		p *= clamp(max(MinRad2 / r2, MinRad2), 0.0, 1.0);  // dp3,div,max.sat,mul
		p = p * scale + p0;
		if (r2 > Bailout) break;
	}
	//return p.w;
	return pow(float(i) / Iterations, 1.0);
}

float fractal_ray_march(vec3 origin, vec3 ray, out bool background_hit) {
	vec3 p = origin;
	float depth = 0.0f;
	int nb_steps = 0;
	float dist1, dist2;
	while (nb_steps < max_steps) {
		nb_steps++;
		float dist = fractal_signed_distance(p + depth * ray);
		if (dist < min_distance) break;
		depth += dist;
		if (depth > max_distance) {
			background_hit = true;
			return depth;
		}
	}
	background_hit = false;
	return depth;
}

vec3 fractal_estimate_normal(vec3 p, float epsilon) {
	return normalize(vec3(
		fractal_signed_distance(vec3(p.x + epsilon, p.y, p.z)) - fractal_signed_distance(vec3(p.x - epsilon, p.y, p.z)),
		fractal_signed_distance(vec3(p.x, p.y + epsilon, p.z)) - fractal_signed_distance(vec3(p.x, p.y - epsilon, p.z)),
		fractal_signed_distance(vec3(p.x, p.y, p.z + epsilon)) - fractal_signed_distance(vec3(p.x, p.y, p.z - epsilon))
	));
}

uniform samplerCube u_skybox;

uniform vec3 u_light_dir;
vec3 light_dir = normalize(vec3(1.0f, 1.00, -1.00));
vec3 light2_dir = normalize(vec3(1.0f, 1.00, 1.00));
vec3 light_col = vec3(1.0f);

vec3 hsv2rgb(vec3 c)
{
	vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
	vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
	return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

const int bouncing = 4;
const float dimming_factor = 1.5f;

void main() {
	bool background_hit;
	vec3 ray = normalize(v_ray);
	vec3 origin = 0.005 * v_origin;

	float depth = fractal_ray_march(origin, ray, background_hit);
	if (background_hit) discard;

	vec3 collision_point = origin + depth * ray;
	vec3 normal = fractal_estimate_normal(collision_point, 0.00001);
	vec3 reflection = normalize(reflect(ray, normal));

	float color = fractal_color(collision_point);

	vec3 normal_color = 0.5f + 0.5f * normal;
	vec3 reflection_color = 0.5f + 0.5f * reflection;

	vec3 diff = reflection_color;//vec3(1.0f) * (dot(normal, light_dir) + dot(normal, light2_dir));

	vec3 frag_color = diff * hsv2rgb(vec3(color, 1.0f, 1.0f));

	fragColor = vec4(frag_color, 1.0f);
}
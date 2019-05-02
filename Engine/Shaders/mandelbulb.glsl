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
	v_ray = (invprojview * vec4(pos * (u_far - u_near), u_far + u_near, u_far - u_near)).xyz;
}

#shader fragment
#version 430 core
#define FLOOR = 0
#define SPHERE 1

in vec3 v_origin;
in vec3 v_ray;

out vec4 fragColor;

uniform mat4 u_proj_matrix;
uniform mat4 u_view_matrix;
uniform vec3 u_cameraPos;
uniform float u_near;
uniform float u_far;
uniform float u_raymarching_max_distance;
uniform float u_raymarching_min_distance;
uniform int u_raymarching_max_steps; 
uniform float u_scale;
uniform float u_MinRad2;
uniform int u_iterations;
uniform float u_bailout;

float max_distance = u_raymarching_max_distance;
float min_distance = u_raymarching_min_distance;
int max_steps = u_raymarching_max_steps;

const int Iterations = u_iterations;
float Bailout = u_bailout;
float Power = 8.0f;

float fractal_signed_distance(vec3 pos) {

	vec3 z = pos;
	float dr = 1.0;
	float r = 0.0;
	for (int i = 0; i < Iterations; i++) {
		r = length(z);
		if (r > Bailout) break;

		// convert to polar coordinates
		float theta = acos(z.z / r);
		float phi = atan(z.y, z.x);
		dr = pow(r, Power - 1.0) * Power * dr + 1.0;

		// scale and rotate the point
		float zr = pow(r, Power);
		theta = theta * Power;
		phi = phi * Power;

		// convert back to cartesian coordinates
		z = zr * vec3(sin(theta)*cos(phi), sin(phi)*sin(theta), cos(theta));
		z += pos;
	}
	return 0.5f * log(r) * r / dr;
}

float fractal_color(vec3 pos) {
	vec3 z = pos;
	float dr = 1.0;
	float r = 0.0;
	float it = 0.0;
	for (int i = 0; i < Iterations; i++) {
		r = length(z);
		if (r > Bailout) break;

		// convert to polar coordinates
		float theta = acos(z.z / r);
		float phi = atan(z.y, z.x);
		dr = pow(r, Power - 1.0) * Power * dr + 1.0;

		// scale and rotate the point
		float zr = pow(r, Power);
		theta = theta * Power;
		phi = phi * Power;

		// convert back to cartesian coordinates
		z = zr * vec3(sin(theta)*cos(phi), sin(phi)*sin(theta), cos(theta));
		z += pos;
		it += 1.0f;
	}
	return it / float(Iterations);
	return 0.5 * log(r) * r / dr;
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
vec3 light_dir = normalize(vec3(-1.0f, 1.00, 1.00));
vec3 light2_dir = normalize(vec3(1.0f, 1.00, -1.00));
vec3 light_col = vec3(1.0f);

vec3 pow(vec3 p, float e) {
	return vec3(pow(p.x, e), pow(p.y, e), pow(p.z, e));
}

vec3 hsv2rgb(vec3 c)
{
	vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
	vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
	return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void mandelbulb_main() {
	bool background_hit;
	vec3 ray = normalize(v_ray);
	vec3 origin = 0.005 * v_origin;

	float depth = fractal_ray_march(origin, ray, background_hit);
	if (background_hit) discard;

	vec3 collision_point = origin + depth * ray;
	vec3 normal = fractal_estimate_normal(collision_point, 0.001);
	vec3 reflection = normalize(reflect(ray, normal));

	float color = fractal_color(collision_point);
	//if (color < 0) discard;
	vec3 normal_color = 0.5f + 0.5f * normal;
	vec3 reflection_color = 0.5f + 0.5f * reflection;

	vec3 diff = vec3(1.0f) * normal_color;//(dot(normal, light_dir) / dot(normal, light2_dir));

	vec3 frag_color = diff * hsv2rgb(vec3(color, 1.0f, 1.0f));

	fragColor = vec4(frag_color, 1.0f);//vec4(diff * vec3(0.5 * color, color, 1.0f - color), 1.0f);//vec4(normal_color * reflection_color, 1.0f);//texture(u_skybox, normal);// *vec4((hsv2rgb(vec3(color, 1.0f, 1.0f))).rgb, 1.0f);
	//fragColor = vec4(vec3(color), 1.0f);
	//fragColor = vec4((pow(0.5 + 0.5 * reflection, 0.5f) * pow(0.5 + 0.5 * normal, 0.5f) * vec3(pow(color, 0.3))).brg, 1.0f);
	//fragColor = vec4(vec3(dot(light_dir, normalize(reflection))), 1.0f);
	//fragColor = texture(u_skybox, reflection);
	//fragColor = vec4(vec3(dot(light_dir, normalize(reflection))), 1.0f) * vec4(0.5 * normalize(collision_point) + 0.5f, 1.0f);
}

const int bouncing = 4;
const float dimming_factor = 1.5f;

void main() {
	mandelbulb_main();
}
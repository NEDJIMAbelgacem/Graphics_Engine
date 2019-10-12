#shader vertex
#version 430 core

layout(location = 0) in vec2 pos;

uniform mat4 u_proj;
uniform mat4 u_view;
uniform float u_near;
uniform float u_far;

uniform vec3 cameraPos;

out vec3 v_origin;
out vec3 v_ray;

void main() {
	gl_Position = vec4(pos, 0.0, 1.0);
	mat4 invprojview = inverse(u_proj * u_view);
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

// uniform float u_light_intensity;
uniform mat4 u_proj;
uniform mat4 u_view;
uniform vec3 u_cameraPos;
uniform float u_near;
uniform float u_far;
/*
uniform vec3 u_sphere_pos;
uniform vec3 u_torus_pos;
uniform vec3 u_cylinder_pos;
*/
float sphere_sdf(vec3 p, vec3 center, float radius) {
	return length(p - center) - radius;
}

float capsule_sdf(vec3 p, vec3 a, vec3 b, float radius) {
	float t = dot(p - a, b - a) / dot(b - a, b - a);
	t = clamp(t, 0.0f, 1.0f);
	return length(p - (a + t * (b - a))) - radius;
}

float torus_sdf(vec3 p, vec3 center, float circle_radius, float e_radius) {
	vec3 p2 = p - center;
	vec2 q = vec2(length(p2.xz) - circle_radius, p2.y);
	return length(q) - e_radius;
}

float box_sdf(vec3 p, vec3 center, vec3 size) {
	return length(max(abs(p - center) - size, 0.0f));
}

float cylinder_sdf(vec3 p, vec3 a, vec3 b, float radius) {
	vec3 ab = b - a;
	float t = dot(p - a, ab) / dot(ab, ab);
	vec3 c = a + t * (b - a);
	float d = length(p - c) - radius;
	float y = (abs(t - 0.5) - 0.5) * length(ab);

	float e = length(max(vec2(d, y), 0.0f));
	float i = min(max(d, y), 0);
	return e + i;
}

float infinite_horizontal_floor_sdf(vec3 p, float y) {
	return p.y - y;
}

float round_operator(float dist, float r) {
	return dist - r;
}

float union_operator(float dist1, float dist2) {
	return min(dist1, dist2);
}

float subtraction_operator(float dist1, float dist2) {
	return max(-dist1, dist2);
}

float intersection_operator(float dist1, float dist2) {
	return max(dist1, dist2);
}

float smooth_union_operator(float dist1, float dist2, float k) {
	float h = clamp(0.5 + 0.5 * (dist2 - dist1) / k, 0.0f, 1.0f);
	return mix(dist2, dist1, h) - k * h * (1.0f - h);
}

float smooth_subtraction_operator(float dist1, float dist2, float k) {
	float h = clamp(0.5 - 0.5 * (dist2 + dist1) / k, 0.0f, 1.0f);
	return mix(dist2, -dist1, h) + k * h * (1.0f - h);
}

float smooth_intersection_operator(float dist1, float dist2, float k) {
	float h = clamp(0.5 - 0.5 * (dist2 - dist1) / k, 0.0f, 1.0f);
	return mix(dist2, dist1, h) + k * h * (1.0f - h);
}

float displacement(vec3 p) {
	float k = 20.0f;
	return sin(k * p.x)*sin(k * p.y)*sin(k * p.z);
}

float displacement_operator(float d1, vec3 p) {
	return d1 + displacement(p);
}

float twist_torus(in vec3 p, vec3 center, float circle_radius, float e_radius) {
	vec3 p2 = p - center;
	const float k = 0.03;
	float c = cos(k*p2.x);
	float s = sin(k*p2.x);
	mat2  m = mat2(c, -s, s, c);
	vec3  q = vec3(m*p2.yz, p2.x);
	return torus_sdf(q, vec3(0.0f), circle_radius, e_radius);
}

float bend_torus(in vec3 p, vec3 center, float circle_radius, float e_radius)
{
	vec3 p2 = p - center;
	const float k = 0.03; // or some other amount
	float c = cos(k*p2.x);
	float s = sin(k*p2.x);
	mat2  m = mat2(c, -s, s, c);
	vec3  q = vec3(m*p2.xy, p2.z);
	return torus_sdf(q, vec3(0.0f), circle_radius, e_radius);
}

float repeate_sphere_operator(in vec3 p, in vec3 c, vec3 sphere_center, float radius)
{
	vec3 q = mod(p - sphere_center, c) - 0.5 * c;
	return sphere_sdf(q, vec3(0.0f), radius);
}

vec3 sphere_center = vec3(-500.0f, 100.0f, -500.0f);
float sphere_radius = 60.0f;
vec3 floor_pos = vec3(0.0f, 0.0f, 0.0f);
vec3 floor_size = vec3(1000.0f, 1.0f, 1000.0f);
float spacing = 250.0f;
int nb_spheres = 5;

vec3 get_closest_sphere_center(vec3 p) {
	vec3 first_tile = sphere_center;// - spacing / 2.0f;
    first_tile.x -= spacing / 2.0f;
    first_tile.z -= spacing / 2.0f;
	p -= first_tile;
	int x = clamp(int(p.x / spacing), 0, nb_spheres);
	int z = clamp(int(p.z / spacing), 0, nb_spheres);
	return sphere_center + vec3(x * spacing, 0, z * spacing);
}


float ray_sphere_intersection(vec3 origin, vec3 ray, vec3 sphere_center, float sphere_radius, out bool hits) {
	float a = ray.x * ray.x + ray.y * ray.y + ray.z * ray.z;
	float b = 2 * (origin.x * ray.x + origin.y * ray.y + origin.z * ray.z);
	float c = origin.x * origin.x + origin.y * origin.y + origin.z * origin.z - sphere_radius * sphere_radius;
	float delta = a * a - 4 * a * c;
	if (delta < 0) {
		hits = false;
		return -1.0f;
	}
	float r_delta = sqrt(delta);
	float x1 = (-b - r_delta) / (2 * a);
	float x2 = (-b + r_delta) / (2 * a);
	float t1 = min(x1, x2);
	float t2 = max(x1, x2);
	if (t2 < 0) {
		hits = false;
		return -1.0f;
	}
	hits = true;
	if (t1 < 0) return t2;
	return t1;
}

float signed_distance(vec3 p) {
	vec3 closest_sphere_center = get_closest_sphere_center(p);
	float sphere_dist = sphere_sdf(p, closest_sphere_center, sphere_radius);;
	float floor_dist = box_sdf(p, floor_pos, floor_size);

	return union_operator(floor_dist, sphere_dist);
}

int closest_object(vec3 p) {
	float floor_dist = box_sdf(p, floor_pos, floor_size);
	vec3 closest_sphere_center = get_closest_sphere_center(p);
	float closest_sphere_dist = sphere_sdf(p, closest_sphere_center, sphere_radius);
    if (floor_dist < closest_sphere_dist) return 0;

	closest_sphere_center -= sphere_center;

	//vec3 first_tile = sphere_center;
    //first_tile.x -= spacing / 2.0f;
    //first_tile.z -= spacing / 2.0f;
	//closest_sphere_center -= first_tile;
	int closest_sphere = int(closest_sphere_center.x / spacing) + nb_spheres * int(closest_sphere_center.z / spacing);

	return 1 + closest_sphere;
}

vec3 estimate_normal(vec3 p, float epsilon) {
	return normalize(vec3(
		signed_distance(vec3(p.x + epsilon, p.y, p.z)) - signed_distance(vec3(p.x - epsilon, p.y, p.z)),
		signed_distance(vec3(p.x, p.y + epsilon, p.z)) - signed_distance(vec3(p.x, p.y - epsilon, p.z)),
		signed_distance(vec3(p.x, p.y, p.z + epsilon)) - signed_distance(vec3(p.x, p.y, p.z - epsilon))
	));
}

// const float max_distance = 2000.0f;
// const int max_steps = 80;
// const float min_distance = 1.0f;

// float march_ray(vec3 origin, vec3 ray, out bool background_hit) {
// 	vec3 p = origin;
// 	float depth = 0.0f;
// 	int nb_steps = 0;
// 	while (nb_steps < max_steps) {
// 		nb_steps++;
// 		float dist = signed_distance(p + depth * ray);
// 		if (dist < min_distance) break;
// 		depth += dist;
// 		if (depth > max_distance) {
// 			background_hit = true;
// 			return depth;
// 		}
// 	}
// 	background_hit = false;
// 	return depth;
// }

const float epsilon = 0.05;
const float tile_size = 100.0f;
const float PI = 3.14159265359;

// F0 : surface reflection
vec3 fresnelSchlick(float cosTheta, vec3 F0) {
	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

float DistributionGGX(vec3 N, vec3 H, float roughness) {
	float a = roughness * roughness;
	float a2 = a * a;
	float NdotH = max(dot(N, H), 0.0);
	float NdotH2 = NdotH * NdotH;

	float num = a2;
	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = PI * denom * denom;

	return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness) {
	float r = (roughness + 1.0);
	float k = (r*r) / 8.0;

	float num = NdotV;
	float denom = NdotV * (1.0 - k) + k;

	return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	float ggx2 = GeometrySchlickGGX(NdotV, roughness);
	float ggx1 = GeometrySchlickGGX(NdotL, roughness);

	return ggx1 * ggx2;
}

uniform float u_metallic;
uniform float u_roughness;
uniform samplerCube u_skybox;
uniform samplerCube u_irradianceMap;
uniform vec3 u_sphere_albedo;
uniform vec3 u_emmited_light;
uniform float u_light_intensity;
uniform bool u_isHDR;
uniform vec3 u_light_dir;
uniform int u_reflection_bounces;

vec3 light_pos = vec3(0.0f, 1000000.0f, 0.0f);
vec3 light_color = vec3(1.0f);
vec3 sphere_albedo = u_sphere_albedo;
float sphere_ambient_occlusion = 1.0f;
float metallic = u_metallic;
float roughness = u_roughness;

vec3 F0;
vec3 calculate_color(int object, vec3 collision_point, vec3 normal, vec3 reflection) {
	vec3 object_albedo;
	if (object == 0) {
		vec2 v = collision_point.xz - vec2(1000.0f);
		vec2 r = floor(2.0f * mod(v, tile_size) / tile_size);
		if ((int(r.x) ^ int(r.y)) == 1) object_albedo = vec3(1.0f);
		else object_albedo = vec3(0.5f);
		metallic = u_metallic;
		roughness = u_roughness;
	}
	else {
		object_albedo = vec3(1.0);//sphere_albedo;
		object -= 1;
		metallic = 0.01 + float(object % (nb_spheres)) / float(nb_spheres + 1);
		roughness = 0.01 + float(object / (nb_spheres)) / float(nb_spheres + 1);
	}
	F0 = mix(vec3(0.03), object_albedo, metallic);

	vec3 view_dir = normalize(u_cameraPos - collision_point);
	vec3 color = vec3(0.0f);
	// -----------------------------------------------------------------------
	// per light calculation
	vec3 light_dir = vec3(0.0f, 1.0f, 0.0f);//normalize(light_pos - collision_point);
	vec3 half_vect = normalize(view_dir + light_dir);
	vec3 light_radiance = light_color;
	if (u_isHDR) {
		light_radiance = texture(u_skybox, normal).rgb;
		light_radiance = light_radiance / (light_radiance + vec3(1.0));
		light_radiance = pow(light_radiance, vec3(1.0 / 2.2));
	} else light_radiance = texture(u_skybox, normal).rgb;
	
	// cook-torrance brdf
	float NDF = DistributionGGX(normal, half_vect, roughness);
	float G = GeometrySmith(normal, view_dir, light_dir, roughness);
	vec3 F = fresnelSchlick(max(dot(half_vect, view_dir), 0.0), F0);

	vec3 numerator = NDF * G * F;
	float denominator = 4.0 * max(dot(normal, view_dir), 0.0) * max(dot(normal, light_dir), 0.0);
	vec3 specular = numerator / max(denominator, 0.001);

	vec3 kS = F;
	vec3 kD = vec3(1.0) - kS;
	kD *= 1.0 - metallic;

	// add to outgoing radiance Lo
	// float NdotL = max(dot(normal, light_dir), 0.0);
	// vec3 Lo = (kD * object_albedo / PI + specular) * light_radiance * NdotL;
    // // ---------------------------------------------------------------------

	// // ambient lighting (we now use IBL as the ambient term)
	// //kS = fresnelSchlick(max(dot(normal, view_dir), 0.0), F0);
	// //kD = 1.0 - kS;
	// //kD *= 1.0 - metallic;
	// //vec3 irradiance = texture(u_skybox, normal).rgb;
	// //vec3 diffuse = irradiance * object_albedo;
	// //vec3 ambient = (kD * diffuse) * sphere_ambient_occlusion;

	// //vec3 Le = dot(view_dir, normal) * u_emmited_light * u_light_intensity;
	// //color += /*Le +*/ ambient + Lo;
	// vec3 ambient = vec3(0.03) * object_albedo * vec3(1.0f);
    // color = Lo;

	float NdotL = max(dot(normal, light_dir), 0.0);
	vec3 Lo = (kD * object_albedo / PI + specular) * light_radiance * NdotL;

	// ambient lighting (we now use IBL as the ambient term)
	kS = fresnelSchlick(max(dot(normal, view_dir), 0.0), F0);
	kD = 1.0 - kS;
	kD *= 1.0 - metallic;
	vec3 irradiance = texture(u_irradianceMap, normal).rgb;
	vec3 diffuse = irradiance * object_albedo;
	vec3 ambient = (kD * diffuse) * sphere_ambient_occlusion;

	color = ambient + Lo;

    //if (u_isHDR) {
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0 / 2.2));
	//}

	return pow(color, vec3(1.0f));
}

vec3 light_dir = normalize(vec3(-1.0f, -1.00, 1.00));
vec3 light_col = vec3(1.0f);

vec3 pow(vec3 p, float e) {
	return vec3(pow(p.x, e), pow(p.y, e), pow(p.z, e));
}



const int bouncing = 3;//u_reflection_bounces;

vec4 get_background_color(in samplerCube background_skybox, in vec3 ray) {
	vec3 bckg_color = texture(u_skybox, ray).rgb;
	// if (u_isHDR) {
		bckg_color = bckg_color / (bckg_color + vec3(1.0));
		bckg_color = pow(bckg_color, vec3(1.0 / 2.2));
	// }
	return bckg_color;
}

const float max_distance = 5000.0f;
const int max_steps = 100;
const float min_distance = 0.1f;

float march_ray(vec3 origin, vec3 ray, out bool background_hit) {
	vec3 p = origin;
	float depth = 0.0f;
	int nb_steps = 0;
	while (nb_steps < max_steps) {
		nb_steps++;
		float dist = signed_distance(p + depth * ray);
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

vec3 raymarching_routine(vec3 origin, vec3 ray, out bool background_hit, out vec3 collision_point, out vec3 normal, out vec3 reflection) {
	float depth = march_ray(origin, ray, background_hit);
	collision_point = origin + depth * ray;
	normal = estimate_normal(collision_point, epsilon);
	reflection = normalize(reflect(ray, normal));
	reflection = vec3(reflection.x, reflection.y, reflection.z);
	if (background_hit) return get_background_color(u_skybox, ray);
	int obj = closest_object(collision_point);
	return calculate_color(obj, collision_point, normal, reflection);
}

void scene1_main() {
	u_light_intensity;
	bool background_hit = false;
	vec3 collision_point = v_origin;
	vec3 ray = normalize(v_ray);
	vec3 normal = vec3(0.0);
	vec3 reflection = vec3(0.0);
	int bounce_limit = bouncing;
	vec3 color = vec3(1.0);

	for (int i = 0; i < bounce_limit; ++i) {
		vec3 collision_point2;
		vec3 c = raymarching_routine(collision_point + 0.1 * ray, ray, background_hit, collision_point2, normal, reflection);;
		color *= c;
		if (background_hit) break;
		
		collision_point = collision_point2;
		ray = reflection;
	}
	fragColor = vec4(color, 1.0);
	return;

	// bool background_hit = false;
	// vec3 collision_point1 = v_origin;
    // vec3 normal1;
    // vec3 reflection1 = normalize(v_ray);
	// vec3 object_color1 = raymarching_routine(v_origin, reflection1, background_hit, collision_point1, normal1, reflection1);
	// if (background_hit) {
    //     vec3 envColor = texture(u_skybox, v_ray).rgb;
    //     //if (u_isHDR) {
    //         envColor = envColor / (envColor + vec3(1.0));
    //         envColor = pow(envColor, vec3(1.0 / 2.2));
    //     //}
    //     fragColor = vec4(envColor, 1.0f);
    //     return;
    // }
    
    // vec3 collision_point2, normal2, reflection2;
	// vec3 object_color2 = vec3(1.0f);
    // int i;
	// for (i = 0; i < bouncing; ++i) {		
	// 	object_color2 = raymarching_routine(collision_point1, reflection1, background_hit, collision_point2, normal2, reflection2);
    //     if (background_hit) break;
    //     object_color1 = object_color1 * object_color2;
	// 	collision_point1 = collision_point2;
	// 	normal1 = normal2;
	// 	reflection1 = reflection2;
	// }
    // vec3 envColor = texture(u_skybox, normal1).rgb;
    // if (u_isHDR) {
    // 	envColor = envColor / (envColor + vec3(1.0));
    // 	envColor = pow(envColor, vec3(1.0 / 2.2));
    // }
    
    // /*if (background_hit) {
		
	// 	fragColor = vec4(object_color1 * envColor, 1.0);
	// 	return;
	// }*/
    // fragColor = vec4(object_color1, 1.0f);
    // return;
}

void main() {
	scene1_main();
}
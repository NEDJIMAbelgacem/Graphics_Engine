#include "Camera.h"


Camera::Camera(float fov, float screen_width, float screen_height, float near_plane, float far_plane, glm::vec3 position, glm::vec3 up, float yaw, float pitch) 
	: camera_front(glm::vec3(0.0f, 0.0f, -1.0f)), movement_speed(SPEED), mouse_sensitivity(SENSITIVITY) {
	this->field_of_view = fov;
    this->screen_height = screen_height;
    this->screen_width = screen_width;
    this->near_plane = near_plane;
    this->far_plane = far_plane;
    this->camera_pos = position;
	this->world_up = up;
	this->yaw = yaw;
	this->pitch = pitch;
	this->UpdateCameraVectors();
}

glm::mat4 Camera::GetProjectionMatrix() {
    return glm::perspective(field_of_view, screen_width / screen_height, near_plane, far_plane);
}

glm::mat4 Camera::GetViewMatrix() {
	return glm::lookAt(camera_pos, camera_pos + camera_front, camera_up);
}

void Camera::SetPosition(glm::vec3 pos) {
	this->camera_pos = pos;
	UpdateCameraVectors();
}

void Camera::ProcessKeyboard(int direction, float delta_time) {
	float velocity = movement_speed * delta_time;
	if (direction == GLFW_KEY_UP)
		camera_pos += camera_front * velocity;
	else if (direction == GLFW_KEY_DOWN)
		camera_pos -= camera_front * velocity;
	else if (direction == GLFW_KEY_LEFT)
		camera_pos -= camera_right * velocity;
	else if (direction == GLFW_KEY_RIGHT)
		camera_pos += camera_right * velocity;
    else return;
	this->UpdateCameraVectors();
}

void Camera::SetAngles(float pitch, float yaw) {
	this->pitch = pitch;
	this->yaw = yaw;
	if (this->pitch >=  180.0f) this->pitch -= 360.0f;
	if (this->pitch <= -180.0f) this->pitch += 360.0f;
	if (this->yaw >=  180.0f) this->yaw -= 360.0f;
	if (this->yaw <= -180.0f) this->yaw += 360.0f;
	this->UpdateCameraVectors();
}

void Camera::GenerateRayFrom(float x, float y, glm::vec3& origin, glm::vec3& ray) {
    float aspect_ratio = this->screen_width / this->screen_height;
    float tan_fov = glm::tan(field_of_view / 2.0f);
    float ndc_x = (x + 0.5f) / this->screen_width;
    float ndc_y = (y + 0.5f) / this->screen_height;
    float pixel_x = (2.0f * ndc_x - 1.0f) * tan_fov * aspect_ratio;
    float pixel_y = (1.0f - 2.0f * ndc_y) * tan_fov;

    glm::vec4 pixel_c = glm::vec4(pixel_x, pixel_y, -1.0f, 1.0f);
    glm::vec4 origin_c = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    glm::mat4 inv_view = glm::inverse(this->GetViewMatrix());

    glm::vec4 ray_start = inv_view * origin_c;
    glm::vec4 ray_end = inv_view * pixel_c;

    origin = glm::vec3(ray_start.x, ray_start.y, ray_start.z);
    ray_end = glm::normalize(ray_end - ray_start);
    ray = glm::normalize(glm::vec3(ray_end.x, ray_end.y, ray_end.z));
}

void Camera::ProcessMouseMove(float delta_x, float delta_y, float delta_time) {
	float pitch_delta = delta_x / screen_width;
	float yaw_delta = delta_y / screen_height;
	SetAngles(pitch + SENSITIVITY * pitch_delta, yaw + SENSITIVITY * yaw_delta);
}

void Camera::UpdateCameraVectors() {
	glm::mat4 rotMat = glm::rotate(glm::mat4(1.0f), glm::radians(pitch), glm::vec3(0.0f, -1.0f, 0.0f));
	rotMat = glm::rotate(rotMat, glm::radians(yaw), glm::vec3(-1.0f, 0.0f, 0.0f));
	camera_front = glm::normalize(rotMat * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));
	camera_right = glm::normalize(glm::cross(camera_front, world_up));
	camera_up = glm::normalize(glm::cross(camera_right, camera_front));
}
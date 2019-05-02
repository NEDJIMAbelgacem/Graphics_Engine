#include "Camera.h"


Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) 
	: cameraFront(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
	cameraPos = position;
	worldUp = up;
	this->yaw = yaw;
	this->pitch = pitch;
	updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() {
	return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

void Camera::SetPosition(glm::vec3 pos) {
	this->cameraPos = pos;
	updateCameraVectors();
}

glm::mat4 Camera::getMirroredViewMatrix() {
	cameraPos = glm::vec3(cameraPos.x, -cameraPos.y, cameraPos.z);
	this->yaw = -yaw;
	updateCameraVectors();
	glm::mat4 res = this->getViewMatrix();
	this->cameraPos = glm::vec3(cameraPos.x, -cameraPos.y, cameraPos.z);
	this->yaw = -yaw;
	updateCameraVectors();
	return res;
}

void Camera::ProcessKeyboard(int direction, float deltaTime) {
	float velocity = MovementSpeed * deltaTime;
	if (direction == GLFW_KEY_UP)
		cameraPos += cameraFront * velocity;
	if (direction == GLFW_KEY_DOWN)
		cameraPos -= cameraFront * velocity;
	if (direction == GLFW_KEY_LEFT)
		cameraPos -= cameraRight * velocity;
	if (direction == GLFW_KEY_RIGHT)
		cameraPos += cameraRight * velocity;
	updateCameraVectors();
}

void Camera::SetAngles(float pitch, float yaw) {
	this->pitch = pitch;
	this->yaw = yaw;
	if (this->pitch >=  180.0f) this->pitch -= 360.0f;
	if (this->pitch <= -180.0f) this->pitch += 360.0f;
	if (this->yaw >=  180.0f) this->yaw -= 360.0f;
	if (this->yaw <= -180.0f) this->yaw += 360.0f;
	/*if (this->pitch <= -MAX_PITCH) 
		this->pitch = -MAX_PITCH + 1.0f;
	if (this->pitch >= MAX_PITCH)
		this->pitch = MAX_PITCH - 1.0f;
	if (this->yaw <= -MAX_YAW)
		this->yaw = -MAX_YAW + 1.0f;
	if (this->yaw >= MAX_YAW)
		this->yaw = MAX_YAW - 1.0f;
		*/
	this->updateCameraVectors();
}

float Camera::GetPitch() {
	return pitch;
}

float Camera::GetYaw() {
	return yaw;
}

void Camera::SetNearFarPlanes(float near_plane, float far_plane) {
    this->near_plane = near_plane;
    this->far_plane = far_plane;
}

void Camera::SetProjectionMatrix(glm::mat4 proj) {
    this->projection_m = proj;
}

void Camera::GenerateRayFrom(float x, float y, glm::vec3& origin, glm::vec3& ray) {
    glm::mat4 view = this->getViewMatrix();
    glm::mat4& proj = this->projection_m;
    glm::mat4 invprojview = glm::inverse(proj * view);
    glm::vec2 p(x / 800.0f, y / 600.0f);
    glm::vec4 origin_v4 = near_plane * invprojview * glm::vec4(p, -1.0, 1.0);
	glm::vec4 ray_v4 = invprojview * glm::vec4(p * (far_plane - near_plane), far_plane + near_plane, far_plane - near_plane);
    origin = glm::vec3(origin_v4.x, origin_v4.y, origin_v4.z);
    ray = glm::vec3(ray_v4.x, ray_v4.y, ray_v4.z);
}

void Camera::ProcessMouseMove(float x, float y) {
	if (firstMove) {
		mouseX = x;
		mouseY = y;
		firstMove = false;
		return;
	}
	float pitchDelta = (mouseX - x) / 800.0f;
	float yawDelta = (mouseY - y) / 600.0f;
	mouseX = x;
	mouseY = y;
	SetAngles(pitch + SENSITIVITY * pitchDelta, yaw + SENSITIVITY * yawDelta);
}

void Camera::SetCameraSpeed(float speed) {
	this->MovementSpeed = speed;
}

glm::vec3 Camera::getCameraPosition() {
	return cameraPos;
}

void Camera::updateCameraVectors()
{
	glm::mat4 rotMat = glm::rotate(glm::mat4(1.0f), glm::radians(pitch), glm::vec3(0.0f, 1.0f, 0.0f));
	rotMat = glm::rotate(rotMat, glm::radians(yaw), glm::vec3(1.0f, 0.0f, 0.0f));
	cameraFront = glm::normalize(rotMat * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));
	cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));
	cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
}
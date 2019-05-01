#pragma once
#include "Core/Common.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#define PI 3.14159f

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 3000.0f;
const float SENSITIVITY = 50.0f;
const float ZOOM = 45.0f;
const float MAX_PITCH = 360.0f;
const float MAX_YAW = 360.0f;

class Camera {
public:
	Camera(glm::vec3 position = glm::vec3(-100.0f, 100.0f, 500.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
	void SetPosition(glm::vec3 pos);
	void SetAngles(float pitch, float yaw);
	void ProcessMouseMove(float x, float y);
	float GetPitch();
	float GetYaw();
	glm::mat4 getViewMatrix();
	glm::mat4 getMirroredViewMatrix();
	glm::vec3 getCameraPosition();
	void SetCameraSpeed(float speed);

	void ProcessKeyboard(int direction, float deltaTime);
private:
	void updateCameraVectors();
private:
	glm::vec3 cameraPos;
	glm::vec3 cameraRight;
	glm::vec3 cameraUp;
	glm::vec3 cameraFront;
	glm::vec3 worldUp;
	float pitch;
	float yaw;
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;
	float stepsForward;
	float stepsAside;
	float mouseX, mouseY;
	bool firstMove = true;
};
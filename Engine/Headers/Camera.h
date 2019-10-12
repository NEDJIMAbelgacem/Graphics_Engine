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

// Pitch and Yaw are probably inverted :-(
// camera angles are in degrees
class Camera {
private:
	glm::vec3 camera_pos;
	glm::vec3 camera_right;
	glm::vec3 camera_up;
	glm::vec3 camera_front;
	glm::vec3 world_up;
	float pitch;
	float yaw;
	float movement_speed;
	float mouse_sensitivity;

    float near_plane, far_plane;
    float field_of_view;
    float screen_width, screen_height;

private:
	void UpdateCameraVectors();
public:
	Camera(float fov = glm::radians(90.0f), float screen_width = WINDOW_WIDTH, float screen_height = WINDOW_HEIGHT, float near_plane = 0.1, float far_plane = 4000.0f, glm::vec3 position = glm::vec3(-100.0f, 100.0f, 500.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
	void SetPosition(glm::vec3 pos);
	void SetAngles(float pitch, float yaw);

	void ProcessMouseMove(float x, float y, float delta_time);
    void ProcessKeyboard(int direction, float delta_time);

	inline float GetPitch() { return this->pitch; }
	inline float GetYaw() { return this->yaw; }

    inline glm::vec3 GetCameraPosition() { return this->camera_pos; }
    inline glm::vec3 GetRightDirection() { return this->camera_right; }
    inline glm::vec3 GetUpDirection() { return this->camera_up; }

    inline float GetCameraSpeed() { return this->movement_speed; }
    inline void SetCameraSpeed(float speed) { this->movement_speed = speed; }

    inline float GetMouseSensivity() { return this->mouse_sensitivity; } 
    inline void SetMouseSensivity(float sensivity) { this->mouse_sensitivity = sensivity; }

    inline float GetNearPlane() { return this->near_plane; }
    inline float GetFarPlane() { return this->far_plane; }
    inline void SetNearFarPlanes(float near_plane, float far_plane) { this->near_plane = near_plane; this->far_plane = far_plane; }
    
    inline float GetFieldOfView() { return this->field_of_view; }
    inline void SetFieldOfView(float fov) { this->field_of_view = fov; }

    inline void SetScreenSize(float width, float height) { this->screen_width = width; this->screen_height = height; }

    glm::mat4 GetProjectionMatrix();
	glm::mat4 GetViewMatrix();

    void GenerateRayFrom(float x, float y, glm::vec3& origin, glm::vec3& ray);
};
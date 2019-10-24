#pragma once
#include "Core/Common.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "AbstractCamera.h"

// Default camera values
const float _YAW = -90.0f;
const float _PITCH = 0.0f;
const float _ZOOM = 45.0f;
const float _MAX_PITCH = 360.0f;
const float _MAX_YAW = 360.0f;

class Camera_3D : public AbstractCamera {
private:
	glm::vec3 camera_pos;
	glm::vec3 camera_right;
	glm::vec3 camera_up;
	glm::vec3 camera_front;
	glm::vec3 world_up;
    float pitch;
	float yaw;

    float near_plane, far_plane;
    float field_of_view;
    float screen_width, screen_height;


    // float zoom_level = 1.0f;
    // float screen_width, screen_height;
private:
    void UpdateCameraVectors() {
        glm::mat4 rotMat = glm::rotate(glm::mat4(1.0f), glm::radians(pitch), glm::vec3(0.0f, -1.0f, 0.0f));
        rotMat = glm::rotate(rotMat, glm::radians(yaw), glm::vec3(-1.0f, 0.0f, 0.0f));
        camera_front = glm::normalize(rotMat * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));
        camera_right = glm::normalize(glm::cross(camera_front, world_up));
        camera_up = glm::normalize(glm::cross(camera_right, camera_front));
    }
public:
	Camera_3D(
            glm::vec3 position, float fov = glm::radians(60.0f), 
            float screen_width = WINDOW_WIDTH, float screen_height = WINDOW_HEIGHT, 
            float near_plane = 0.1, float far_plane = 4000.0f, 
            glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH
        ) 
        : camera_front(glm::vec3(0.0f, 0.0f, -1.0f)) {
        camera_front = glm::normalize(-position);
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
    void SetPosition(glm::vec3 pos) {
        this->camera_pos = pos;
	    UpdateCameraVectors();
    }

    glm::vec3 GetPosition() { return this->camera_pos; }

	void SetAngles(float pitch, float yaw) {
        this->pitch = pitch;
        this->yaw = yaw;
        if (this->pitch >=  180.0f) this->pitch -= 360.0f;
        if (this->pitch <= -180.0f) this->pitch += 360.0f;
        if (this->yaw >=  180.0f) this->yaw -= 360.0f;
        if (this->yaw <= -180.0f) this->yaw += 360.0f;
        this->UpdateCameraVectors();
    }

	inline float GetPitch() { return this->pitch; }
	inline float GetYaw() { return this->yaw; }

    inline glm::vec3 GetCameraPosition() { return this->camera_pos; }
    inline glm::vec3 GetRightDirection() { return this->camera_right; }
    inline glm::vec3 GetUpDirection() { return this->camera_up; }

    inline float GetNearPlane() { return this->near_plane; }
    inline float GetFarPlane() { return this->far_plane; }
    inline void SetNearFarPlanes(float near_plane, float far_plane) { this->near_plane = near_plane; this->far_plane = far_plane; }
    
    inline float GetFieldOfView() { return this->field_of_view; }
    inline void SetFieldOfView(float fov) { this->field_of_view = fov; }

    inline void SetScreenSize(float width, float height) { this->screen_width = width; this->screen_height = height; }
    inline float GetScreenWidth() { return screen_width; }
    inline float GetScreenHeight() { return screen_height; }

    glm::mat4 GetProjectionMatrix() {
        return glm::perspective(field_of_view, screen_width / screen_height, near_plane, far_plane);
    }

	glm::mat4 GetViewMatrix() {
        // UpdateCameraVectors();
        // if (glm::dot(camera_up, glm::vec3(0.0f, 1.0f, 0.0f)) > 0.0f) {
        //     N3D_CORE_TRACE("{}", -1);
        //     return glm::lookAt(camera_pos, glm::vec3(0.0f), -camera_up);
        // }
        // N3D_CORE_TRACE("{}", 1);
	    return glm::lookAt(camera_pos, glm::vec3(0.0f), world_up);
	    // return glm::lookAt(camera_pos, camera_pos + camera_front, camera_up);
    }

    glm::vec3 GetCameraUp() {
        return camera_up;
    }

    void MoveUp(float distance) {
        camera_pos += camera_up * distance;
    }

    void MoveDown(float distance) {
        camera_pos -= camera_up * distance;
    }

    void MoveRight(float distance) {
        camera_pos += camera_right * distance;
    }

    void MoveLeft(float distance) {
        camera_pos -= camera_right * distance;
    }

    void MoveForward(float distance) {
        camera_pos += glm::normalize(camera_pos) * distance;
        // camera_pos += camera_front * distance;
    }

    void MoveBackward(float distance) {
        camera_pos += camera_front * distance;
    }

    void FillShader(ShaderProgram& prg) override {
        glm::mat4 view_m = this->GetViewMatrix();
        glm::mat4 proj_m = this->GetProjectionMatrix();
        prg.FillUniformMat4f("u_view", view_m);
        prg.FillUniformMat4f("u_proj", proj_m);
    }
    std::pair<glm::vec3, glm::vec3> GenerateRayFrom(float screen_space_x, float screen_space_y) {
        glm::vec3 ray;
        auto transform_to_projection_space = [](glm::vec4 p, float width, float height) -> glm::vec4 {
            glm::mat4 scale = glm::scale(glm::identity<glm::mat4>(), glm::vec3(2.0f / width, -2.0f / height, 1));
            glm::vec4 v = scale * p;
            glm::mat4 translation = glm::translate(glm::identity<glm::mat4>(), glm::vec3(-1, 1, 0));
            v = translation * v;
            return translation * scale * p;
        };

        glm::vec4 projection_space_p = transform_to_projection_space(glm::vec4(screen_space_x, screen_space_y, 0.01f, 1.0f), this->screen_width, this->screen_height);

        glm::vec4 view_space_p = glm::inverse(this->GetProjectionMatrix()) * projection_space_p;
        view_space_p.w = 1.0f;
        glm::vec4 world_space_p = glm::inverse(this->GetViewMatrix()) * view_space_p;
        world_space_p.w = 1.0f;
        ray = glm::vec3(world_space_p.x - camera_pos.x, world_space_p.y - camera_pos.y, world_space_p.z - camera_pos.z);
        return {camera_pos, glm::normalize(ray)};
    }
};
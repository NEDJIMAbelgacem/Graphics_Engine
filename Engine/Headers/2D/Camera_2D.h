#pragma once
#include "Core/Common.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "AbstractCamera.h"

class Camera_2D : public AbstractCamera {
private:
	glm::vec2 camera_pos;
	glm::vec2 camera_right = glm::vec2(-1.0f, 0.0f);
	glm::vec2 camera_up = glm::vec2(0.0f, 1.0f);
    float zoom_level = 1.0f;
    float screen_width, screen_height;
public:
	Camera_2D(float screen_width = WINDOW_WIDTH, float screen_height = WINDOW_HEIGHT, glm::vec2 position = glm::vec2(0.0f, 0.0f)) {
        this->screen_height = screen_height;
        this->screen_width = screen_width;
        this->camera_pos = position;
    }

	void SetPosition(glm::vec2 pos) { this->camera_pos = pos; }

    void ZoomIn(float zoom_factor) {
        zoom_level *= zoom_factor;
    }

    void ZoomOut(float zoom_factor) {
        zoom_level /= zoom_factor;
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

    inline glm::vec2 GetCameraPosition() { 
        // return glm::vec2(0.5f * screen_width + camera_pos.x, 0.5 * screen_height + camera_pos.y);
        return this->camera_pos;
    }
    
    inline void SetScreenSize(float width, float height) { this->screen_width = width; this->screen_height = height; }
    inline float GetScreenWidth() { return screen_width; }
    inline float GetScreenHeight() { return screen_height; }

    glm::mat4 GetViewMatrix() override {
        glm::mat4 scale = glm::scale(glm::identity<glm::mat4>(), glm::vec3(1.0f / zoom_level, 1.0f / zoom_level, 1.0f));
        glm::mat4 translation = glm::translate(glm::identity<glm::mat4>(), glm::vec3(camera_pos.x, camera_pos.y, 0.0f));
        return translation * scale;
    }

	glm::mat4 GetProjectionMatrix() override {
        glm::mat4 scale = glm::scale(glm::identity<glm::mat4>(), glm::vec3(2.0f / screen_width, -2.0f / screen_height, 1.0f));
        glm::mat4 translation = glm::translate(glm::identity<glm::mat4>(), glm::vec3(-1.0f, 1.0f, 0.0f));
        glm::mat4 m = translation * scale;
        return m;
    }

    void FillShader(ShaderProgram& prg) override {
        glm::mat4 view_m = this->GetViewMatrix();
        glm::mat4 proj_m = this->GetProjectionMatrix();
        prg.FillUniformMat4f("u_view", view_m);
        prg.FillUniformMat4f("u_proj", proj_m);

    }
};
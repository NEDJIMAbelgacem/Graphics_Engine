
#pragma once
#include "Core/Common.h"
#include "Layers/Layer.h"
#include "Window/Application.h"
#include "3D/Camera_3D.h"
#include "3D/3D_Objects.h"

// const char* single_color_shader_path = "Shaders/3D/single_colored.glsl";
const char* single_color_shader_path = "Shaders/3D/mine_sphere.glsl";
const char* sky_shader_path = "Shaders/3D/skybox.glsl";
using namespace N3D;

class RenderingLayer_3D : public Layer {
private:
    Camera_3D& camera;
    std::vector<Object_3D*> objects;
    ShaderProgram* single_color_shader = nullptr;
    
    ShaderProgram* sky_shader = nullptr;
    N3D::SkyBox* skybox = nullptr;

    glm::vec3 background_color;
    bool depth_test_enabled = false;
public:
    RenderingLayer_3D(Camera_3D& _camera) : camera(_camera) {
        single_color_shader = new ShaderProgram(single_color_shader_path);
        sky_shader = new ShaderProgram(sky_shader_path);
        EnableDepthTest();
    }

    ~RenderingLayer_3D() {
        delete single_color_shader;
    }

    void EnableDepthTest() {
        if (!depth_test_enabled) {
            depth_test_enabled = true;
            glEnable(GL_DEPTH_TEST);
        }
    }

    void DisableDepthTest() {
        if (depth_test_enabled) {
            depth_test_enabled = false;
            glDisable(GL_DEPTH_TEST);
        }
    }

    bool IsDepthTestEnable() { return depth_test_enabled; }

    void SetBackgroundColor(glm::vec3 color) { this->background_color = color; }
    glm::vec3 GetBackgroundColor() { return this->background_color; }

    void Add_3D_Object(Object_3D& obj) {
        objects.push_back(&obj);
    }

    void SetSkybox(N3D::SkyBox& skybox) {
        this->skybox = &skybox;
    }

    void OnUpdate() override {
        glClearColor(background_color.r, background_color.g, background_color.b, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        camera.FillShader(*single_color_shader);
        if (skybox != nullptr) {
            camera.FillShader(*sky_shader);
            // sky_shader->FillUniform1f("u_near", camera.GetNearPlane());
            // sky_shader->FillUniform1f("u_near", camera.GetFarPlane());
            sky_shader->FillUniformVec3("u_cameraPos", camera.GetCameraPosition());
            skybox->FillShader(*sky_shader);

            sky_shader->Bind();
            glDepthMask(GL_FALSE);
            skybox->Render();
            glDepthMask(GL_TRUE);
            sky_shader->Unbind();
        }
        for (auto obj : objects) {
            obj->FillShader(*single_color_shader);
            single_color_shader->Bind();
            obj->Render();
            single_color_shader->Unbind();
        }
	}
};
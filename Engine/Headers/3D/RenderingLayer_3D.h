
#pragma once
#include "Core/Common.h"
#include "Layers/Layer.h"
#include "Window/Application.h"
#include "3D/Camera_3D.h"
#include "3D/3D_Objects.h"
#include "Font.h"

// const char* single_color_shader_path = "Shaders/3D/single_colored.glsl";
const char* single_color_shader_path = "Shaders/3D/mine_sphere.glsl";
const char* sky_shader_path = "Shaders/3D/skybox.glsl";
const char* text_shader_path = "Shaders/2D/text_rendering.glsl";
const char* text_3d_shader_path = "Shaders/3D/text_3d.glsl";

using namespace N3D;

class RenderingLayer_3D : public Layer {
private:
    Camera_3D& camera;
    std::vector<Object_3D*> objects;
    ShaderProgram* single_color_shader = nullptr;

    ShaderProgram* sky_shader = nullptr;
    N3D::SkyBox* skybox = nullptr;

    ShaderProgram* text_shader = nullptr;
    Font* text_font = nullptr;
    std::vector<Text*> text_v;

    ShaderProgram* text_3d_shader = nullptr;
    std::vector<Text_3D*> text3d_v;

    glm::vec3 background_color;
    bool depth_test_enabled = false;
public:
    RenderingLayer_3D(Camera_3D& _camera) : camera(_camera) {
        single_color_shader = new ShaderProgram(single_color_shader_path);
        sky_shader = new ShaderProgram(sky_shader_path);
        text_shader = new ShaderProgram(text_shader_path);
        text_3d_shader = new ShaderProgram(text_3d_shader_path);
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
        camera.FillShader(*text_3d_shader);
        text_shader->FillUniformMat4f("u_proj", glm::ortho(0.0f, camera.GetScreenWidth(), 0.0f, camera.GetScreenHeight()));
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        if (text_font != nullptr)
        for (auto& txt : text_v) {
            txt->FillShader(*text_shader);
            text_shader->Bind();
            txt->Render(*text_font);
            text_shader->Unbind();
        }
        if (text_font != nullptr)
        for (auto& txt : text3d_v) {
            txt->FillShader(*text_3d_shader);
            text_3d_shader->Bind();
            txt->Render(*text_font);
            text_3d_shader->Unbind();
        }
	}

    void SetTextFont(Font& font) {
        text_font = &font;
    }

    void AddText(std::string text, glm::vec2 pos, float scale, glm::vec3 color = glm::vec3(0.0f)) {
        text_v.push_back(new Text(pos, scale, text, color));
    }

    void AddText3D(Text_3D& txt) {
        text3d_v.push_back(&txt);
    }
};
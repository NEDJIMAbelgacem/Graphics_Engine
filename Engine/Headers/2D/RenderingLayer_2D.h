
#pragma once
#include "Core/Common.h"
#include "Layers/Layer.h"
#include "Systems/RenderingManager.h"
#include "Systems/ComponentsManager.h"
#include "Window/Application.h"
#include "Geometry/Cube.h"
#include "Geometry/Plane.h"
#include "Geometry/Sphere.h"
#include "2D/Camera_2D.h"
#include "2D/Objects/Object_2D.h"

const char* single_color_shader_path = "Shaders/2D/single_colored.glsl";
using namespace N3D;

class RenderingLayer_2D : public Layer {
private:
    AbstractCamera& camera;
    std::vector<Object_2D*> objects;
    ShaderProgram* single_color_shader = nullptr;
    glm::vec3 background_color;
public:
    RenderingLayer_2D(AbstractCamera& _camera) : camera(_camera) {
        single_color_shader = new ShaderProgram(single_color_shader_path);
    }

    void SetBackgroundColor(glm::vec3 color) { this->background_color = color; }
    glm::vec3 GetBackgroundColor() { return this->background_color; }

    void Add2DObject(Object_2D& obj) {
        objects.push_back(&obj);
    }

    ~RenderingLayer_2D() {
        delete single_color_shader;
    }

    void OnUpdate() override {
        glClearColor(background_color.r, background_color.g, background_color.b, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        camera.FillShader(*single_color_shader);
        for (auto obj : objects) {
            obj->FillShader(*single_color_shader);
            single_color_shader->Bind();
            obj->Render();
            single_color_shader->Unbind();
        }
	}
};
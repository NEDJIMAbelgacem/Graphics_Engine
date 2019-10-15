
#pragma once
#include "Core/Common.h"
#include "Layers/Layer.h"
#include "Systems/RenderingManager.h"
#include "Systems/ComponentsManager.h"
#include "Window/Application.h"
#include "Geometry/Cube.h"
#include "Geometry/Plane.h"
#include "Geometry/Sphere.h"

class RenderingLayer : public Layer {
private:
    Camera* camera;
    SkyBox* skybox;
public:
    RenderingLayer(Camera* camera) {
        this->camera = camera;
    }

    ~RenderingLayer() { }

    void SetSkyBox(SkyBox& skybox) {
        this->skybox = &skybox;
        RenderingManager::RegisterObject(skybox);
    }

    void AddGameObject(GameObject& obj) {
        RenderingManager::RegisterObject(obj);
    }

    void AddModel(Model& model) {
        RenderingManager::RegisterObject(model);
    }
    
    // TODO : add batch rendering
    void AddBatchRender(Model& model, Batch& batch) {
    }

    void AddSphere(Sphere& sphere) {
        RenderingManager::RegisterObject(sphere);
    }

    void AddCube(Cube& cube) {
        RenderingManager::RegisterObject(cube);
    }

    void AddPlane(Plane& plane) {
        RenderingManager::RegisterObject(plane);
    }

    void OnUpdate() override {
        RenderingManager::Update(Application::delta_time);
        RenderingManager::UpdateCamera(*camera);
        RenderingManager::Render();
	}
};
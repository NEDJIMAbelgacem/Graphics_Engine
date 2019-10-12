#pragma once
#include "Core/Common.h"
#include "Model.h"
#include "ShaderComponents/Shaded.h"
#include "SkyBox.h"
#include "Components/Component.h"
#include "Systems/ComponentsManager.h"
#include "Light.h"

class RenderingManager {
private:
    static std::set<GameObject*> game_objects;
    static std::set<std::pair<Model*, Batch*>> batches;
public:
    static void RegisterModelBatch(Model& model, Batch& batch);
    static void RegisterObject(GameObject& obj);
    static void UnregisterObject(GameObject& obj);

    static void Update(float delta);
    static void UpdateCamera(Camera& camera);

    static void Render();
};
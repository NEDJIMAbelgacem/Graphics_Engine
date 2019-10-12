#include "Systems/RenderingManager.h"

std::set<GameObject*> RenderingManager::game_objects;
std::set<std::pair<Model*, Batch*>> RenderingManager::batches;

void RenderingManager::RegisterModelBatch(Model& model, Batch& batch) {
    std::pair<Model*, Batch*> p(&model, &batch);
    if (batches.find(p) != batches.end()) return;
    batches.insert(p);
    ComponentsManager::RegisterObject(*(GameObject*)&model);
}

void RenderingManager::RegisterObject(GameObject& obj) {
    if (game_objects.find(&obj) != game_objects.end()) return;
    game_objects.insert(&obj);
    ComponentsManager::RegisterObject(obj);
}

void RenderingManager::UnregisterObject(GameObject& obj) {
    if (game_objects.find(&obj) == game_objects.end()) return;
    ComponentsManager::UnregisterObject(obj);
    game_objects.erase(&obj);
}

void RenderingManager::Update(float delta) {
    for (GameObject* obj : game_objects) obj->Update(delta);
    for (GameObject* obj : game_objects) {
        ShaderProgram& shader = *obj->GetShader();
        for (auto it = obj->begin(); it != obj->end(); ++it) {
            Component* c = it->second;
            ComponentsManager::UpdateComponent(shader, *c);
        }
    }
}

void RenderingManager::UpdateCamera(Camera& camera) {
    glm::mat4 proj = camera.GetProjectionMatrix();
    glm::mat4 view = camera.GetViewMatrix();
}

void RenderingManager::Render() {
    for (GameObject* m : game_objects) m->Render();
    for (std::pair<Model*, Batch*> p : batches) p.first->RenderBatch(*p.second);
}
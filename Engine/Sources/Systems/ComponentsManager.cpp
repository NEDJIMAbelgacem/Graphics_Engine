#include "Systems/ComponentsManager.h"

std::unordered_map<int, std::set<GameObject*>> ComponentsManager::objects_per_component;

void ComponentsManager::RegisterObject(GameObject& obj) {
    for (auto it = obj.begin(); it != obj.end(); ++it) {
        Component* c = it->second;
        ComponentsManager::objects_per_component[c->GetID()].insert(&obj);
    }
}

void ComponentsManager::UnregisterObject(GameObject& obj) {
    for (auto it = obj.begin(); it != obj.end(); ++it) {
        Component* c = it->second;
        ComponentsManager::objects_per_component[c->GetID()].erase(&obj);
    }
}

void ComponentsManager::AddObjectComponent(GameObject& obj, Component& component) {
    ComponentsManager::objects_per_component[component.GetID()].insert(&obj);
}

void ComponentsManager::EraseObjectComponent(GameObject& obj, Component& component) {
    int component_id = component.GetID();
    std::set<GameObject*>& obj_set = ComponentsManager::objects_per_component[component_id];
    if (obj_set.find(&obj) != obj_set.end()) 
        obj_set.erase(&obj);
}

void ComponentsManager::UpdateComponent(ShaderProgram& shader, Lighting& component) {
    LightingManager::UpdateShader(shader, component);
}

void ComponentsManager::UpdateComponent(ShaderProgram& shader, Surface& component) {
	shader.FillUniform1f("u_material.reflectivity", component.reflectivity);
	shader.FillUniform1f("u_material.shineDamper", component.shine_damper);
    shader.FillUniform1f("u_material.diffuseFactor", component.diffuse_factor);
}

void ComponentsManager::UpdateComponent(ShaderProgram& shader, Transform& component) {
    glm::vec3 scale = component.scale;
    glm::mat4 scale_m = glm::scale(glm::identity<glm::mat4>(), component.scale);
    glm::mat4 rot_m = glm::identity<glm::mat4>();
    rot_m *= glm::rotate(glm::identity<glm::mat4>(), component.rotation[0], glm::vec3(1.0f, 0.0f, 0.0f));
    rot_m *= glm::rotate(glm::identity<glm::mat4>(), component.rotation[1], glm::vec3(0.0f, 1.0f, 0.0f));
    rot_m *= glm::rotate(glm::identity<glm::mat4>(), component.rotation[2], glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 trans_m = glm::translate(glm::identity<glm::mat4>(), component.position);
    shader.FillUniformMat4f("u_model", trans_m * rot_m * scale_m);
}

void ComponentsManager::UpdateComponent(ShaderProgram& shader, CameraHook& component) {
	shader.FillUniformMat4f("u_view", component.camera->GetViewMatrix());
    shader.FillUniformMat4f("u_proj", component.camera->GetProjectionMatrix());
    shader.FillUniform1f("u_near", component.camera->GetNearPlane());
    shader.FillUniform1f("u_far", component.camera->GetFarPlane());
    glm::vec3 cameraPos = component.camera->GetCameraPosition();
    shader.FillUniform3f("u_cameraPos", cameraPos.x, cameraPos.y, cameraPos.z);
}

void ComponentsManager::UpdateComponent(ShaderProgram& shader, ShaderParameters& component) {
    component.UpdateShaders(shader);
}

void ComponentsManager::UpdateComponent(ShaderProgram& shader, Component& component) {
    switch ((ComponentTypes)component.GetID()) {
    case ComponentTypes::Lighting:
        UpdateComponent(shader, *(Lighting*)&component);
        break;
    case ComponentTypes::Surface:
        UpdateComponent(shader, *(Surface*)&component);
        break;
    case ComponentTypes::Transform:
        UpdateComponent(shader, *(Transform*)&component);
        break;
    case ComponentTypes::CameraHook:
        UpdateComponent(shader, *(CameraHook*)&component);
        break;
    case ComponentTypes::ShaderParameters:
        UpdateComponent(shader, *(ShaderParameters*)&component);
        break;
    }
}

#include "Systems/LightingManager.h"

std::vector<Light*> LightingManager::scene_lights;

void LightingManager::AddLight(Light& light) {
    LightingManager::scene_lights.push_back(&light);
}

void LightingManager::UpdateShader(ShaderProgram& shader, Lighting& component) {
    shader.FillUniform1i("u_lights_count", (int)LightingManager::scene_lights.size());
    for (int i = 0; i < LightingManager::scene_lights.size(); ++i) {
        Light& l = *scene_lights[i];
        std::string alias = std::string("u_lights[") + std::to_string(i) + "]";
        shader.FillUniform1i(alias + ".type", (int)l.type);
        shader.FillUniformVec3(alias + ".color", l.color);
        shader.FillUniformVec3(alias + ".position", l.position);
    }
}

std::vector<Light*>& LightingManager::GetLightsVector() {
    return LightingManager::scene_lights;
}
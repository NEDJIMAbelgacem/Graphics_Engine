#pragma once
#include "Core/Common.h"
#include "Model.h"
#include "ShaderComponents/Shaded.h"
#include "SkyBox.h"
#include "Components/Component.h"
#include "Systems/ComponentsManager.h"
#include "Light.h"
#include "Components/Lighting.h"

class LightingManager {
private:
    static std::vector<Light*> scene_lights;
public:
    static void AddLight(Light& light);
    static void UpdateShader(ShaderProgram& shader, Lighting& component);
    static std::vector<Light*>& GetLightsVector();
};
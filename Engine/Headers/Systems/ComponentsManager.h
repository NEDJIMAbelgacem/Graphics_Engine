#pragma once
#include "Core/Common.h"
#include "Core/ShaderProgram.h"
#include "GameObject.h"
#include "Components/Component.h"
#include "Components/CameraHook.h"
#include "Components/Lighting.h"
#include "Components/Surface.h"
#include "Components/Transform.h"
#include "Components/ShaderParameters.h"
#include "Systems/LightingManager.h"

class ComponentsManager {
private:
    static std::unordered_map<int, std::set<GameObject*>> objects_per_component;
    static void UpdateComponent(ShaderProgram& shader, Lighting& component);
    static void UpdateComponent(ShaderProgram& shader, Surface& component);
    static void UpdateComponent(ShaderProgram& shader, Transform& component);
    static void UpdateComponent(ShaderProgram& shader, CameraHook& component);
    static void UpdateComponent(ShaderProgram& shader, ShaderParameters& component);
public:
    static void RegisterObject(GameObject& obj);
    static void UnregisterObject(GameObject& obj);
    static void AddObjectComponent(GameObject& obj, Component& component);
    static void EraseObjectComponent(GameObject& obj, Component& component);
    static void UpdateComponent(ShaderProgram& shader, Component& component);
};
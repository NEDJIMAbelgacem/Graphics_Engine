#pragma once
#include "Core/Common.h"
#include "Components/Component.h"
#include "Core/ShaderProgram.h"

// GameObjects hold the ownership of it Component data
// There exist one component per type
class GameObject {
protected:
    std::unordered_map<int, Component*> components;
    ShaderProgram* shader = nullptr;
public:
    GameObject();
    ~GameObject();

    ShaderProgram* GetShader() { return this->shader; }
    virtual void Update(float delta) { }
    virtual void Render() { }

    Component* GetComponent(int component_type);
    void AddComponent(Component* component);
    void EraseComponent(int component_type);

    std::unordered_map<int, Component*>::iterator begin();
    std::unordered_map<int, Component*>::iterator end();
};
#include "GameObject.h"

GameObject::GameObject() { }

GameObject::~GameObject() {
    for (auto& i : components) delete i.second;
}

Component* GameObject::GetComponent(int component_type) { 
    return components[component_type];
}

void GameObject::AddComponent(Component* component) {
    component->owner = this;
    int component_type = component->GetID();
    components.emplace(component_type, component);
}

std::unordered_map<int, Component*>::iterator GameObject::begin() {
    return components.begin();
}

std::unordered_map<int, Component*>::iterator GameObject::end() {
    return components.end();
}
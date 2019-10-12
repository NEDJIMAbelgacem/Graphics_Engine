#pragma once
#include "Core/Common.h"

class GameObject;

struct Component {
    friend class GameObject;
protected:
    int id;
    GameObject* owner = nullptr;
public:
    int GetID() { return this->id; }
    virtual void Update(float delta) { }
};
#pragma once
#include "Components/Component.h"
#include "Components/ComponentTypes.h"
#include "Camera.h"

class CameraHook : public Component {
public:
    Camera* camera;

    CameraHook(Camera& camera) {
        this->camera = &camera;
        this->id = (int)ComponentTypes::CameraHook;
    }
};
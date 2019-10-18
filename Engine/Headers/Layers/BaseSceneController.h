#pragma once
#include <functional>
#include <vector>

#include "Layers/Layer.h"
#include "Logger.h"
#include "2D/Objects/Object_2D.h"
#include "2D/Camera_2D.h"

using namespace N3D;

// BaseSceneController is responsible of propagating events to the scene controller
// and provide an interface for polymorphism
class BaseSceneController : public Layer {
protected:
    AbstractCamera& camera;
public:
	BaseSceneController(AbstractCamera& _camera, std::string layer_name = "Scene view control layer") 
    : Layer(layer_name), camera(_camera) { }

    virtual bool OnMouseButtonPressed(MouseButtonPressedEvent& e) { return false; }

    virtual bool OnMouseButtonReleased(MouseButtonReleasedEvent& e) { return false; }

    virtual bool OnMouseMoved(MouseMovedEvent& e) { return false; }

	void OnEvent(Event& event) override {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(BaseSceneController::OnMouseMoved));
        dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(BaseSceneController::OnMouseButtonPressed));
        dispatcher.Dispatch<MouseButtonReleasedEvent>(BIND_EVENT_FN(BaseSceneController::OnMouseButtonReleased));
	}
};
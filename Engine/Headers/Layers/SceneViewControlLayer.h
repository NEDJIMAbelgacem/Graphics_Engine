#pragma once
#include <functional>

#include "Layers/Layer.h"
#include "Logger.h"

// TODO : fix events propagation system
// ouseMoved_actions are a function that takes a delta x, y values and returns a boolean which determines if the event will be consumed or propagated to lower layers
// SceneViewControlLayer is responsible of handling mouse usage and propagating events to a scene
class SceneViewControlLayer : public Layer {
private:
    float curr_mouse_x = -1, curr_mouse_y = -1;
    float dx, dy;
    std::vector<std::function<bool(MouseMovedEvent&)>> mouseMoved_actions;
    std::vector<std::function<bool(MouseButtonPressedEvent&)>> mouseButtonPressed_actions;
    std::vector<std::function<bool(MouseButtonReleasedEvent&)>> mouseButtonReleased_actions;
public:
	SceneViewControlLayer() : Layer("Keyboard control layer") { }

    void AddMouseMovedAction(std::function<bool(MouseMovedEvent&)> func) {
        mouseMoved_actions.push_back(func);
    }

    void AddMouseButtonPressedAction(std::function<bool(MouseButtonPressedEvent&)> func) {
        mouseButtonPressed_actions.push_back(func);
    }

    void AddMouseButtonReleasedAction(std::function<bool(MouseButtonReleasedEvent&)> func) {
        mouseButtonReleased_actions.push_back(func);
    }

    bool OnMouseMoved(MouseMovedEvent& e) {
        bool consumed = false;
        for (auto& func : mouseMoved_actions) {
            if (func(e)) consumed = true;
        }
        return consumed;
    }

    bool OnMouseButtonPressed(MouseButtonPressedEvent& e) {
        bool consumed = false;
        for (auto& func : mouseButtonPressed_actions) {
            if (func(e)) consumed = true;
        }
        return consumed;
    }

    bool OnMouseButtonReleased(MouseButtonReleasedEvent& e) {
        bool consumed = false;
        for (auto& func : mouseButtonReleased_actions) {
            if (func(e)) consumed = true;
        }
        return consumed;
    }

	void OnEvent(Event& event) override {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(SceneViewControlLayer::OnMouseMoved));
        dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(SceneViewControlLayer::OnMouseButtonPressed));
        dispatcher.Dispatch<MouseButtonReleasedEvent>(BIND_EVENT_FN(SceneViewControlLayer::OnMouseButtonReleased));
	}
};
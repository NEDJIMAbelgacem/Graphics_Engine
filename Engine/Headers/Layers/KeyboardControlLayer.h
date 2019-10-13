#pragma once
#include <functional>

#include "Layers/Layer.h"
#include "Logger.h"
// TODO : handle other key event types (typed / pressed)
class KeyBoardControlLayer : public Layer {
private:
    std::map<int, std::vector<std::function<void()>>> key_actions_map;
public:
	KeyBoardControlLayer() : Layer("Keyboard control layer") { }

    void AddKeyAction(int key_code, std::function<void()> func) {
        key_actions_map[key_code].push_back(func);
    }

    bool OnKeyEvent(KeyEvent& e) {
        for (auto& [key, func_vect] : key_actions_map) {
            if (e.GetKeyCode() == key) {
                for (auto& func : func_vect) func();
                return true;
            }
        }
        return false;
    }

	void OnEvent(Event& event) override {
        EventDispatcher dispatcher(event);
        bool success = false;
        if (!success) success = dispatcher.Dispatch<KeyReleasedEvent>(BIND_EVENT_FN(KeyBoardControlLayer::OnKeyEvent));
        if (!success) success = dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(KeyBoardControlLayer::OnKeyEvent));
        if (!success) success = dispatcher.Dispatch<KeyTypedEvent>(BIND_EVENT_FN(KeyBoardControlLayer::OnKeyEvent));
	}
};
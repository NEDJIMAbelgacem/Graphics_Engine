#pragma once

#include "Core/Common.h"
#include "Events/Event.h"

class Layer {
protected:
    std::string debug_name;
public:
    Layer(const std::string& name = "Layer");
    virtual ~Layer();

    virtual void OnAttach() {}
    virtual void OnDetach() {}
    virtual void OnUpdate() {}
    virtual void OnImGuiRender() {}
    virtual void OnEvent(Event& event) {}

    inline const std::string& GetName() const { return debug_name; }
};

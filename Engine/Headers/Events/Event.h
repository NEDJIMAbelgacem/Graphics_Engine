#pragma once
#include "Core/Common.h"

enum class EventType {
    None = 0,
    WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
    AppTick, AppUpdate, AppRender,
    KeyPressed, KeyReleased, KeyTyped,
    MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,
    ImGuiEvent,
    ImGui1FloatSliderChanged, ImGui3FloatSliderChanged, ImGuiColorPickerChanged,
    ImGui1IntegerSliderChanged,
    ImGui1FloatDragChanged, ImGui2FloatDragChanged, ImGui3FloatDragChanged,
    ImGuiButtonClicked
};

enum EventCategory {
    None = 0,
    EventCategoryApplication    = 1 << 0,
    EventCategoryInput          = 1 << 1,
    EventCategoryKeyboard       = 1 << 2,
    EventCategoryMouse          = 1 << 3,
    EventCategoryMouseButton    = 1 << 4,
    EventCategoryImGuiEvent     = 1 << 5
};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
                            virtual EventType GetEventType() const override { return GetStaticType(); }\
                            virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

class Event {
public:
    bool Handled = false;

    virtual EventType GetEventType() const = 0;
    virtual const char* GetName() const = 0;
    virtual int GetCategoryFlags() const = 0;
    virtual std::string ToString() const { return GetName(); }

    inline bool IsInCategory(EventCategory category) { return GetCategoryFlags() & category; }
};

class EventDispatcher {
    template<typename T>
    using EventFn = std::function<bool(T&)>;
private:
    Event& m_Event;
public:
    EventDispatcher(Event& event) : m_Event(event) { }

    template<typename T>
    bool Dispatch(EventFn<T> func) {
        if (m_Event.GetEventType() == T::GetStaticType()) {
            //m_Event.Handled = 
            func(*(T*)&m_Event);
            if (m_Event.Handled) return true;
        }
        return false;
    }
};

inline std::ostream& operator<<(std::ostream& os, const Event& e) { return os << e.ToString(); }

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)
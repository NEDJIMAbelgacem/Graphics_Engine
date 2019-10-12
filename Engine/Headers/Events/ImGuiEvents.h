#pragma once
#include "Events/Event.h"

class ImGuiEvent : public Event {
public:
    ImGuiEvent() { }

    std::string ToString() const override {
        return "ImGuiEvent";
    }

    EVENT_CLASS_TYPE(ImGuiEvent)
    EVENT_CLASS_CATEGORY(EventCategoryImGuiEvent)
};

class ImGui1FloatSliderChangedEvent : public ImGuiEvent {
private:
    float value;
public:
    ImGui1FloatSliderChangedEvent(float v) : value(v) {}

    inline float GetValue() const { return value; }

    std::string ToString() const override {
        std::stringstream ss;
        ss << "ImGui1FloatSliderChangedEvent: " << value;
        return ss.str();
    }

    EVENT_CLASS_TYPE(ImGui1FloatSliderChanged)
};

class ImGui3FloatSliderChangedEvent : public ImGuiEvent {
private:
    glm::vec3 value;
public:
    ImGui3FloatSliderChangedEvent(glm::vec3 v) : value(v) { }

    inline glm::vec3 GetValue() const { return value; }

    std::string ToString() const override {
        std::stringstream ss;
        ss << "ImGui3FloatSliderChangedEvent: (" << value.x << ", " << value.y << ", " << value.z << ")";
        return ss.str();
    }

    EVENT_CLASS_TYPE(ImGui3FloatSliderChanged)
};

class ImGuiColorPickerChangedEvent : public ImGuiEvent {
private:
    glm::vec3 value;
public:
    ImGuiColorPickerChangedEvent(glm::vec3 v) : value(v) {}

    inline glm::vec3 GetValue() const { return value; }

    std::string ToString() const override {
        std::stringstream ss;
        ss << "ImGuiColorPickerChangedEvent: (" << value.x << ", " << value.y << ", " << value.z << ")";
        return ss.str();
    }

    EVENT_CLASS_TYPE(ImGuiColorPickerChanged)
};

class ImGui1IntegerSliderChangedEvent : public ImGuiEvent {
private:
    int value;
public:
    ImGui1IntegerSliderChangedEvent(int v) : value(v) { }

    inline int GetValue() const { return value; }

    std::string ToString() const override {
        std::stringstream ss;
        ss << "ImGui1IntegerSliderChangedEvent: (" << value << ")";
        return ss.str();
    }

    EVENT_CLASS_TYPE(ImGui1IntegerSliderChanged)
};

class ImGui1FloatDragChangedEvent : public ImGuiEvent {
private:
    float value;
public:
    ImGui1FloatDragChangedEvent(float v) : value(v) { }

    inline float GetValue() const { return value; }

    std::string ToString() const override {
        std::stringstream ss;
        ss << "ImGui1FloatDragChangedEvent: (" << value << ")";
        return ss.str();
    }

    EVENT_CLASS_TYPE(ImGui1FloatDragChanged)
    EVENT_CLASS_CATEGORY(EventCategoryImGuiEvent)
};

class ImGui2FloatDragChangedEvent : public ImGuiEvent {
private:
    glm::vec2 value;
public:
    ImGui2FloatDragChangedEvent(glm::vec2 v) : value(v) { }

    inline glm::vec2 GetValue() const { return value; }

    std::string ToString() const override {
        std::stringstream ss;
        ss << "ImGui2FloatDragChangedEvent: (" << value.x << ", " << value.y << ")";
        return ss.str();
    }

    EVENT_CLASS_TYPE(ImGui2FloatDragChanged)
};

class ImGui3FloatDragChangedEvent : public ImGuiEvent {
private:
    glm::vec3 value;
public:
    ImGui3FloatDragChangedEvent(glm::vec3 v) : value(v) { }

    inline glm::vec3 GetValue() const { return value; }

    std::string ToString() const override {
        std::stringstream ss;
        ss << "ImGui3FloatDragChangedEvent: (" << value.x << ", " << value.y << ", " << value.z << ")";
        return ss.str();
    }

    EVENT_CLASS_TYPE(ImGui3FloatDragChanged)
};

class ImGuiButtonClickedEvent : public ImGuiEvent {
public:
    ImGuiButtonClickedEvent() { }
    
    std::string ToString() const override { return "ImGuiButtonClickedEvent"; }

    EVENT_CLASS_TYPE(ImGuiButtonClicked)
};
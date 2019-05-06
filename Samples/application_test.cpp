#include "Core/Common.h"
#include "Window/Application.h"
#include "Logger.h"
#include "Window/Input.h"
#include "Tests/ImGuiParams.h"

class ExampleLayer : public Layer {
public:
	ExampleLayer() : Layer("Example") { }

	void OnUpdate() override {
		if (Input::IsKeyPressed(GLFW_KEY_TAB)) N3D_LOG_TRACE("Tab key is pressed (poll)!");
	}

	virtual void OnImGuiRender() override {
		ImGui::Begin("Test");
		ImGui::Text("Hello World");
		ImGui::End();
	}

	/*void OnEvent(Event& event) override {
		if (event.GetEventType() == EventType::KeyPressed) {
			KeyPressedEvent& e = (KeyPressedEvent&)event;
			if (e.GetKeyCode() == GLFW_KEY_TAB)
				N3D_LOG_TRACE("Tab key is pressed (event)!");
			else N3D_LOG_TRACE("{0}", (char)e.GetKeyCode());
		}
	}*/
};

class Sandbox : public Application {
public:
	Sandbox() {
		PushLayer(new ImGuiParams("debug"));
	}

	~Sandbox() { }

};

Application* CreateApplication() {
    return new Sandbox();
}

int main(int argc, char** argv) {
	Logger::Init();
	N3D_LOG_TRACE("Initialized Log!");
	N3D_LOG_TRACE("Hello! Var={0}");

	auto app = CreateApplication();

	app->Run();
	delete app;
}
#include "Window/Application.h"
#include <glad/glad.h>
#include "Window/Input.h"

Application* Application::instance = nullptr;

Application::Application() {
    if (instance) N3D_LOG_FATAL("Application already exists!");
    instance = this;

    window = std::unique_ptr<Window>(new Window({"application class test", WINDOW_WIDTH, WINDOW_HEIGHT}));
    window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

    imgui_layer = new ImGuiActivationLayer();
	PushOverlay(imgui_layer);
}

Application::~Application() { }

void Application::PushLayer(Layer* layer) {
    layer_stack.PushLayer(layer);
    layer->OnAttach();
}

void Application::PushOverlay(Layer* layer) {
    layer_stack.PushOverlay(layer);
    layer->OnAttach();
}

void Application::OnEvent(Event& e) {
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));

    for (auto it = layer_stack.end(); it != layer_stack.begin(); ){
        (*--it)->OnEvent(e);
        if (e.Handled) 
            break;
    }
}

float Application::delta_time = 0.0f;
void Application::Run() {
    float time1 = 0.0f, time2 = (float)glfwGetTime();
    while (is_running) {
        time1 = time2;
        time2 = (float)glfwGetTime();
        delta_time = time2 - time1;
        
        glClearColor(1, 0, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (Layer* layer : layer_stack)
            layer->OnUpdate();

        // start a new ImGui frame
        imgui_layer->Begin();
        // ImGui::ShowDemoWindow();

        for (Layer* layer : layer_stack)
            layer->OnImGuiRender();
        // end ImGui frame rendering
        imgui_layer->End();

        window->OnUpdate();
    }
}

bool Application::OnWindowClose(WindowCloseEvent& e) {
    N3D_LOG_TRACE("Close called");
    is_running = false;
    return true;
}

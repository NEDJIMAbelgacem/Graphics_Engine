#include "Window/Application.h"
#include <glad/glad.h>
#include "Window/Input.h"

Application* Application::s_Instance = nullptr;

Application::Application() {
    if (s_Instance) N3D_LOG_FATAL("Application already exists!");
    s_Instance = this;

    m_Window = std::unique_ptr<Window>(new Window({"application class test", 800, 600}));
    m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

    m_ImGuiLayer = new ImGuiLayer();
	PushOverlay(m_ImGuiLayer);
}

Application::~Application() { }

void Application::PushLayer(Layer* layer) {
    m_LayerStack.PushLayer(layer);
    layer->OnAttach();
}

void Application::PushOverlay(Layer* layer) {
    m_LayerStack.PushOverlay(layer);
    layer->OnAttach();
}

void Application::OnEvent(Event& e) {
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));

    for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); ){
        (*--it)->OnEvent(e);
        if (e.Handled) 
            break;
    }
}

float Application::delta_time = 0.0f;
void Application::Run() {
    float time1 = 0.0f, time2 = (float)glfwGetTime();
    while (m_Running) {
        time1 = time2;
        time2 = (float)glfwGetTime();
        delta_time = time2 - time1;
        
        glClearColor(1, 0, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (Layer* layer : m_LayerStack)
            layer->OnUpdate();

        m_ImGuiLayer->Begin();
        for (Layer* layer : m_LayerStack)
            layer->OnImGuiRender();
        m_ImGuiLayer->End();

        m_Window->OnUpdate();
    }
}

bool Application::OnWindowClose(WindowCloseEvent& e) {
    N3D_LOG_TRACE("close called");
    m_Running = false;
    return true;
}

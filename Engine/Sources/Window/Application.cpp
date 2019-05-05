#include "Window/Application.h"
#include <glad/glad.h>
#include "Window/Input.h"

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

Application* Application::s_Instance = nullptr;

Application::Application() {
    if (s_Instance) N3D_LOG_FATAL("Application already exists!");
    s_Instance = this;

    m_Window = std::unique_ptr<Window>(new Window({"application class test", 800, 600}));
    m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
}

Application::~Application() { }

void Application::OnEvent(Event& e) {
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
    //dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
}

void Application::Run()
{
    while (m_Running)
    {
        glClearColor(1, 0, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        m_Window->OnUpdate();
    }
}

bool Application::OnWindowClose(WindowCloseEvent& e)
{
    N3D_LOG_TRACE("close called");
    //m_Running = false;
    return true;
}

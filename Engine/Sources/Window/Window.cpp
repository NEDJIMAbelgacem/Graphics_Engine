#include "Window/Window.h"

static bool s_GLFWInitialized = false;
static void GLFWErrorCallback(int error, const char* description) {
    N3D_LOG_ERROR("GLFW Error ({}): {}", error, description);
}

Window::Window(const WindowProps& props) {
    m_Data.Title = props.Title;
    m_Data.Width = props.Width;
    m_Data.Height = props.Height;

    N3D_LOG_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

    if (!s_GLFWInitialized) {
        // TODO: glfwTerminate on system shutdown
        int success = glfwInit();
        if (!success) {
            N3D_LOG_FATAL("GLFW Fatal Error : Could not intialize GLFW!");
        }
        glfwSetErrorCallback(GLFWErrorCallback);
        s_GLFWInitialized = true;
    }

    m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
    glfwMakeContextCurrent(m_Window);
    int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    if (!status) N3D_LOG_FATAL("Failed to initialize Glad!");
    glfwSetWindowUserPointer(m_Window, &m_Data);
    SetVSync(true);

    // Set GLFW callbacks
    glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        data.Width = width;
        data.Height = height;

        // event system stuff
        WindowResizeEvent event(width, height);
        data.EventCallback(event);
    });

    glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
    {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        
        WindowCloseEvent event;
        data.EventCallback(event);
    });

    glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        // key events
        switch (action) {
            case GLFW_PRESS: 
            {
                KeyPressedEvent event(key, 0);
                data.EventCallback(event);
                break;
            }
            case GLFW_RELEASE:
            {
                KeyReleasedEvent event(key);
                data.EventCallback(event);
                break;
            }
            case GLFW_REPEAT:
            {
                KeyPressedEvent event(key, 1);
                data.EventCallback(event);
                break;
            }
        }
    });

    glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        KeyTypedEvent event(keycode);
        data.EventCallback(event);
    });

    glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        switch (action)
        {
            case GLFW_PRESS:
            {
                MouseButtonPressedEvent event(button);
                data.EventCallback(event);
                break;
            }
            case GLFW_RELEASE:
            {
                MouseButtonReleasedEvent event(button);
                data.EventCallback(event);
                break;
            }
        }
    });

    glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        MouseScrolledEvent event((float)xOffset, (float)yOffset);
        data.EventCallback(event);
    });

    glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        MouseMovedEvent event((float)xPos, (float)yPos);
        data.EventCallback(event);
    });
}

Window::~Window() {
    glfwDestroyWindow(m_Window);
}

void Window::OnUpdate() {
    glfwPollEvents();
    glfwSwapBuffers(m_Window);
}

unsigned int Window::GetWidth() {
    return m_Data.Width;
}

unsigned int Window::GetHeight() {
    return m_Data.Height;
}

void Window::SetVSync(bool enabled) {
    if (enabled) glfwSwapInterval(1);
    else glfwSwapInterval(0);
    m_Data.VSync = enabled;
}

bool Window::IsVSync() {
    return m_Data.VSync;
}
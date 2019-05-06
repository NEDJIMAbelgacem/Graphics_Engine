#include "Core/Common.h"
#include "Window/ImGuiLayer.h"
#include "Window/Application.h"
#include "Window/Events/Event.h"

ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer") { }

ImGuiLayer::~ImGuiLayer() { }

void ImGuiLayer::OnAttach() {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    Application& app = Application::Get();
    GLFWwindow* window = app.GetWindow().GetNativeWindow();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 410");
}

void ImGuiLayer::OnDetach() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiLayer::OnEvent(Event& event) {
    // bool handled = false;
    // EventDispatcher dispatcher(event);
    // dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(OnMouseMoved));
    // dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(OnMouseButtonPressed));
    // dispatcher.Dispatch<MouseButtonReleasedEvent>(BIND_EVENT_FN(OnMouseButtonReleased));
    // dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(OnMouseScrolled));
    // dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(OnKeyPressed));
    // dispatcher.Dispatch<KeyReleasedEvent>(BIND_EVENT_FN(OnKeyReleased));
    // dispatcher.Dispatch<KeyTypedEvent>(BIND_EVENT_FN(OnKeyTyped));
}

// bool ImGuiLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e) {
//     N3D_LOG_INFO("{} : {}", this->GetName(), e);
//     return false;
// }

// bool ImGuiLayer::OnMouseButtonReleased(MouseButtonReleasedEvent& e) {
//     N3D_LOG_INFO("{} : {}", this->GetName(), e);
//     return false;
// }

// bool ImGuiLayer::OnKeyPressed(KeyPressedEvent& e) {
//     N3D_LOG_INFO("{} : {}", this->GetName(), e);
//     return false;
// }

// bool ImGuiLayer::OnKeyReleased(KeyReleasedEvent& e) {
//     N3D_LOG_INFO("{} : {}", this->GetName(), e);
//     return false;
// }

// bool ImGuiLayer::OnKeyTyped(KeyTypedEvent& e) {
//     N3D_LOG_INFO("{} : {}", this->GetName(), e);
//     return false;
// }

// bool ImGuiLayer::OnMouseMoved(MouseMovedEvent& e) {
//     N3D_LOG_INFO("{} : {}", this->GetName(), e.GetName(), e);
//     return false;
// }

// bool ImGuiLayer::OnMouseScrolled(MouseScrolledEvent& e) {
//     N3D_LOG_INFO("{} : {}", this->GetName(), e);
//     return false;
// }

void ImGuiLayer::Begin() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImGuiLayer::End() {
    ImGuiIO& io = ImGui::GetIO();
    Application& app = Application::Get();
    io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

void ImGuiLayer::OnImGuiRender() {
    static bool show = true;
    // if (show) ImGui::ShowDemoWindow(&show);
}
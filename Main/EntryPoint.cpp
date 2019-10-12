#include "Core/Common.h"
#include "Window/Application.h"

Application* CreateApplication() {
    return new Application();
}

int main(int argc, char** argv) {
	Logger::Init();
	N3D_LOG_TRACE("Initialized Log!");

	auto app = CreateApplication();

	app->Run();
	delete app;
}
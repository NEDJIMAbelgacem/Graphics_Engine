#include "Core/Common.h"
#include "Window/Application.h"
#include "Logger.h"

Application* CreateApplication() {
    return new Application();
}

int main(int argc, char** argv) {
	Logger::Init();
	N3D_LOG_TRACE("Initialized Log!");
	N3D_LOG_TRACE("Hello! Var={0}");

	auto app = CreateApplication();

	app->Run();
	delete app;
}
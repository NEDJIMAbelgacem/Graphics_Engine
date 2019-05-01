// openGL-Testing.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
/*
#include "pch.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "BufferLayout.h"
#include "VertexArray.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "Gui.h"
#include "Camera.h"
#include "Test.h"
#include "ModelMatrixTest.h"
#include "Mesh.h"
#include "LightTest.h"
#include "Model.h"
#include "FrameBuffer.h"
#include "Terrain.h"
#include "Grid.h"
#include "Batch.h"
#include "DebugTest.h"
#include "AlphabetAtlas.h"
#include "WaterPlane.h"
#include "TerrainTest.h"

#define PI 3.14159f

#define sign(x) (2.0f * (x) - 1.0f)

void InitImGui(GLFWwindow* window, const char* glsl_version);

void DestroyImGui();

int main()
{
	GLFWwindow* window;

	// initialise glfw
	if (!glfwInit()) return -1;

	// set opengl version to 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_SAMPLES, 4);

	// Create a windowed mode window and its OpenGL context
	window = glfwCreateWindow(800, 600, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	// Make the window's context current
	glfwMakeContextCurrent(window);

	// initialise glew
	if (glewInit() != GLEW_OK) {
		std::cout << "glew initialisation error" << std::endl;
		return -1;
	}
	//glfwSwapInterval(1);

	// Display opengl current version
	std::cout << "opengl version : " << glGetString(GL_VERSION) << std::endl;

	float time1 = glfwGetTime();
	float time2 = glfwGetTime();

	// ImGui context
	{
		ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);

		float near_plane = 0.1f;
		float far_plane = -2000.1f;
		float aspect_ratio = 800.0f / 600.0f;
		float field_of_view = glm::radians(45.0f);

		glm::mat4 proj = glm::perspective(field_of_view, aspect_ratio, near_plane, far_plane);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 model = glm::mat4(1.0f);

		InitImGui(window, "#version 130");
		Renderer renderer;
		VertexArray vao;
		Camera camera;
		camera.SetAngles(0.0f, 0.0f);
		view = camera.getViewMatrix();
		Grid grid(750.0f, 0.0f, 750.0f, 50.0f, 30, "resources/shaders/line_shader.shader");

		// quad stuff
		std::vector<glm::vec3> quad_vertices;
		quad_vertices.push_back(glm::vec3(-50.0f, 10.0f, 50.0f));
		quad_vertices.push_back(glm::vec3(50.0f, 10.0f, 50.0f));
		quad_vertices.push_back(glm::vec3(50.0f, 10.0f, -50.0f));
		quad_vertices.push_back(glm::vec3(-50.0f, 10.0f, -50.0f));
		std::vector<glm::vec2> quad_texCoords;
		quad_texCoords.push_back(glm::vec2(0.0f, 0.0f));
		quad_texCoords.push_back(glm::vec2(0.0f, 1.0f));
		quad_texCoords.push_back(glm::vec2(1.0f, 1.0f));
		quad_texCoords.push_back(glm::vec2(1.0f, 0.0f));
		Quad quad(quad_vertices, quad_texCoords, "resources/shaders/quad_shader.shader", "resources/textures/container2.png", "resources/textures/container2_specular.png");

		bool cursor_disabled = false;
		DebugTest debugTest;
		ModelMatrixTest modelTest;
		LightTest lightTest;
		TerrainTest terrainTest;

		bool mouseEnabled = false;

		std::vector<Test*> tests;
		tests.push_back(&debugTest);
		tests.push_back(&modelTest);
		tests.push_back(&lightTest);
		tests.push_back(&terrainTest);

		int test_index = 0;

		// load post processing shaders
		std::vector<ShaderProgram*> screen_shaders;
		screen_shaders.push_back(new ShaderProgram("resources/shaders/screen_shader.shader"));
		screen_shaders.push_back(new ShaderProgram("resources/shaders/screen_grayscale_shader.shader"));
		screen_shaders.push_back(new ShaderProgram("resources/shaders/screen_inversion_shader.shader"));
		screen_shaders.push_back(new ShaderProgram("resources/shaders/screen_kernel_shader.shader"));
		screen_shaders.push_back(new ShaderProgram("resources/shaders/screen_blur_shader.shader"));
		screen_shaders.push_back(new ShaderProgram("resources/shaders/screen_edge_shader.shader"));
		int screen_shader_index = 0;
		for (int i = 3; i < screen_shaders.size(); ++i)
			screen_shaders[i]->FillUniform1f("offset", 0.0025);

		FrameBuffer frame_buffer;
		float quadVertices[] = {
			-1.0f,  1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,

			-1.0f,  1.0f,  0.0f, 1.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,
			 1.0f,  1.0f,  1.0f, 1.0f
		};
		VertexArray screenVAO;
		VertexBuffer vbo(quadVertices, sizeof(quadVertices));
		BufferLayout layout;
		layout.AddElement<float>(2, 0);
		layout.AddElement<float>(2, 1);
		screenVAO.AddBuffer(vbo, layout);

		for (int i = 0; i < 6; ++i) {
			quadVertices[4 * i] = 0.4 * quadVertices[4 * i] - 0.5;
			quadVertices[4 * i + 1] = 0.4 * quadVertices[4 * i + 1] + 0.5;
		}
		VertexArray textureVAO;
		VertexBuffer vbo1(quadVertices, sizeof(quadVertices));
		BufferLayout layout1;
		layout1.AddElement<float>(2, 0);
		layout1.AddElement<float>(2, 1);
		textureVAO.AddBuffer(vbo1, layout1);

		// load skybox
		std::vector<std::string> faces{
			"resources/textures/skybox/right.jpg",
			"resources/textures/skybox/left.jpg",
			"resources/textures/skybox/top.jpg",
			"resources/textures/skybox/bottom.jpg",
			"resources/textures/skybox/front.jpg",
			"resources/textures/skybox/back.jpg"
		};

		ColoredCube lamp("resources/shaders/box_shader.shader");
		SkyBox skybox(faces, "resources/shaders/skybox_shader.shader");
		ShaderProgram cube_shader("resources/shaders/cube_shader.shader");
		//Texture cube_texture("resources/textures/container2.png");

		// terrain stuff
		int tiles_per_side = 10;
		float tile_length = 200.0f;
		int vertices_per_tile = 20;
		float terrain_length = tiles_per_side * tile_length;
		std::vector<std::string> terrain_textures{
			"resources/textures/grass.png",
			"resources/textures/path.png"
		};
		Terrain terrain(terrain_textures, "resources/shaders/terrain_shader.shader", tiles_per_side, vertices_per_tile, tile_length, 400.0f, 3);
		glm::vec3 start_pos(-terrain_length / 2.0f, 0.0f, -terrain_length / 2.0f);
		glm::vec3 offset(tile_length / 2.0f, 0.0f, tile_length / 2.0f);
		std::vector<glm::vec3> positions;
		int t_size = terrain_length;
		for (int i = 0; i < (tiles_per_side - 3) * (tiles_per_side - 3); ++i) {
			float tile_x = std::abs(rand() % t_size);
			float tile_z = std::abs(rand() % t_size);
			float tile_y = terrain.GetHeight(tile_x / terrain_length, tile_z / terrain_length);
			if (tile_y <= 0) {
				i--;
				continue;
			}
			positions.push_back(glm::vec3(tile_x + start_pos.x, tile_y, tile_z + start_pos.z));
		}

		// load model
		Model obj("resources/meshes/tree.obj", "resources/shaders/model_shader.shader", "resources/meshes/tree.png");
		Batch batch(&obj, positions);
		AlphabetAtlas alpha("resources/alphabet/", "resources/shaders/alphabet_shader.shader");

		// water plane
		WaterPlane water("resources/shaders/water_shader.shader", "resources/textures/water/DUDV.png", "resources/textures/water/normal.png", glm::vec3(0.0f), terrain_length);
		water.SetNearFarPlanes(near_plane, far_plane);

		// draw data
		glm::vec3 lightPos;
		Material material;
		glm::vec3 lightColor;
		glm::vec3 cameraPos;

		bool draw_water_plane = false;
		// Loop until the user closes the window 
		auto draw_frame_fun = [&](glm::vec3 cameraPos, glm::mat4 view) -> void {

			renderer.SetMatrices(view, proj);
			renderer.DrawSkyBox(skybox, vao);

			obj.SetModelMatrix(glm::scale(glm::mat4(1.0f), glm::vec3(10.0f, 10.0f, 10.0f)));
			obj.SetSurfaceParameters(0, 128, material.diffuseFactor);
			obj.SetLightParameters(lightPos, lightColor);
			obj.SetCameraPosition(cameraPos);
			renderer.DrawBatch(batch, vao);

			//renderer.DrawPlaneGrid(grid, vao);
			terrain.SetSurfaceParameters(0, 99999, material.diffuseFactor);
			terrain.SetLightParameters(lightPos, lightColor);
			terrain.SetCameraPosition(cameraPos);
			renderer.DrawTerrain(terrain, vao);

			lamp.SetPosition(lightPos);
			lamp.SetScale(glm::vec3(10.0f, 10.0f, 10.0f));
			lamp.SetColor(lightColor);
			renderer.DrawCube(lamp, vao);

			alpha.SetSurfaceParameters(0, 128, material.diffuseFactor);
			alpha.SetLightParameters(lightPos, lightColor);
			alpha.SetCameraPosition(cameraPos);
			alpha.Draw(renderer, vao, glm::vec3(-50, 100, 0), "SHIT");

			if (draw_water_plane) {
				water.SetCameraPosition(cameraPos);
				water.SetLightParameters(lightPos, lightColor);
				water.SetDistortionStrength(terrainTest.GetDistortionStrength());
				water.SetWaterSpeed(terrainTest.GetWaterSpeed());
				water.SetDepthParameter(terrainTest.GetDepthParameter());
				water.SetSpecularDamper(terrainTest.GetSpecularDamper());
				water.SetReflectivity(terrainTest.GetReflectivity());
				water.SetClampFactor(terrainTest.GetClampFactor());
				water.SetTiling(terrainTest.GetWaterTiling());
				renderer.DrawWaterPlane(water, vao);
			}
		};

		float clip_offset = -15.0f;
		while (!glfwWindowShouldClose(window))
		{
			// Poll for and process events
			glfwPollEvents();

			// update time delta (for camera movement)
			float deltatime = time2 - time1;
			time1 = time2;
			time2 = glfwGetTime();

			bool mouse_toggle_pressed = ImGui::IsKeyPressed(GLFW_KEY_F1);
			bool switch_pressed = ImGui::IsKeyPressed(GLFW_KEY_F2);
			bool up_key_pressed = ImGui::IsKeyPressed(GLFW_KEY_UP) || ImGui::IsKeyPressed(GLFW_KEY_W);
			bool down_key_pressed = ImGui::IsKeyPressed(GLFW_KEY_DOWN) || ImGui::IsKeyPressed(GLFW_KEY_S);
			bool left_key_pressed = ImGui::IsKeyPressed(GLFW_KEY_LEFT) || ImGui::IsKeyPressed(GLFW_KEY_A);
			bool right_key_pressed = ImGui::IsKeyPressed(GLFW_KEY_RIGHT) || ImGui::IsKeyPressed(GLFW_KEY_D);
			bool left_ctrl_pressed = ImGui::IsKeyPressed(GLFW_KEY_LEFT_CONTROL);
			bool f3_pressed = ImGui::IsKeyPressed(GLFW_KEY_F3);

			if (f3_pressed)
				screen_shader_index = (screen_shader_index + 1) % screen_shaders.size();

			if (left_ctrl_pressed) {
				if (cursor_disabled) glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				else glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				cursor_disabled = !cursor_disabled;
			}

			if (switch_pressed) test_index = (test_index + 1) % tests.size();
			if (up_key_pressed) camera.ProcessKeyboard(GLFW_KEY_UP, deltatime);
			if (down_key_pressed) camera.ProcessKeyboard(GLFW_KEY_DOWN, deltatime);
			if (left_key_pressed) camera.ProcessKeyboard(GLFW_KEY_LEFT, deltatime);
			if (right_key_pressed) camera.ProcessKeyboard(GLFW_KEY_RIGHT, deltatime);
			if (mouse_toggle_pressed) mouseEnabled = !mouseEnabled;
			if (mouseEnabled) {
				ImVec2 mousePos = ImGui::GetMousePos();
				camera.ProcessMouseMove(mousePos.x, mousePos.y);
				view = camera.getViewMatrix();
			}

			// update lighting and camera parameters
			model = modelTest.getModelMatrix();
			lightPos = lightTest.GetLightPosition();
			lightColor = lightTest.GetLightColor();
			cameraPos = camera.getCameraPosition();
			material = lightTest.GetMaterial();
			float amplitude = terrainTest.GetTerrainAmplitude();
			terrain.SetHeightAmplitude(amplitude);
			clip_offset = terrainTest.GetClipFactor();

			glCall(glEnable(GL_DEPTH_TEST));
			// refraction
			FrameBuffer* below_wp = water.GetBelowFrameBuffer();
			below_wp->Bind();
			renderer.ClearScr();
			renderer.SetClipPlane(glm::vec4(0.0f, -1.0f, 0.0f, -clip_offset));
			renderer.EnableClipping();
			draw_frame_fun(cameraPos, view);
			renderer.DisableClipping();
			below_wp->Unbind();
			//

			// reflection
			FrameBuffer* above_wp = water.GetAboveFrameBuffer();
			above_wp->Bind();
			renderer.ClearScr();
			renderer.SetClipPlane(glm::vec4(0.0f, 1.0f, 0.0f, clip_offset));
			renderer.EnableClipping();
			draw_frame_fun(cameraPos, camera.getMirroredViewMatrix());
			renderer.DisableClipping();
			above_wp->Unbind();
			//

			// main frame buffer
			frame_buffer.Bind();
			renderer.ClearScr();
			draw_water_plane = true;
			draw_frame_fun(cameraPos, view);
			draw_water_plane = false;
			frame_buffer.Unbind();
			//

			// drawing into screen
			glCall(glDisable(GL_DEPTH_TEST));
			renderer.ClearScr();

			// postprocessing
			frame_buffer.Draw(screenVAO, *screen_shaders[screen_shader_index]);
			//above_wp->Draw(textureVAO, *screen_shaders[screen_shader_index]);

			// update debug data
			debugTest.AddText("camera position", std::string("(") + std::to_string(cameraPos[0]) + ", " + std::to_string(cameraPos[1]) + ", " + std::to_string(cameraPos[2]) + std::string(")"));
			debugTest.AddText("camera yaw", std::to_string(camera.GetPitch()));
			debugTest.AddText("camera pitch", std::to_string(camera.GetYaw()));

			tests[test_index]->Display();
			tests[test_index]->Render();

			// Swap front and back buffers
			glfwSwapBuffers(window);
		}
		DestroyImGui();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

float lastX, lastY;

void InitImGui(GLFWwindow* window, const char* glsl_version) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
}

void DestroyImGui() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}*/
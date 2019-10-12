#include "Tests/DebugTest.h"
#include <ctime>
#include "stb_image_write.h"

DebugTest::DebugTest() { }

float* RotateImage(float* data, int x, int y, int nb_c, bool anti) {
	if (x != y) N3D_LOG_WARN("cube map face not a square and I haven't implemented that path yet :3");
	float* data2 = new float[x * y * nb_c];
	for (int i = 0; i < x; ++i) {
		for (int j = 0; j < y; ++j) {
			int index1 = i * y + j;
			int index2 = (y - 1 - index1 % y) * x + index1 / y;
			for (int k = 0; k < nb_c; ++k) {
				if (anti) data2[nb_c * index1 + k] = data[nb_c * index2 + k];
				else data2[nb_c * index2 + k] = data[nb_c * index1 + k];
			}
		}
	}
	delete data;
	return data2;
}

void SaveTo(std::string image_path) {
	const int width = WINDOW_WIDTH;
	const int height = WINDOW_HEIGHT;
	float* data = new float[3 * width * height];
	glCall(glReadPixels(-1, -1, width, height, GL_RGB, GL_UNSIGNED_BYTE, data));
	//float* data = RotateImage(data, width, height, 3);
	//FlipImage(data, width, height, 3);
	stbi_write_png(image_path.c_str(), width, height, 3, data, 0);
	delete data;
}

void DebugTest::Display() {
    ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Debug");
	for (std::string s : texts_order) {
		std::string line = s + " : " + texts[s];
		ImGui::Text(line.c_str());
		ImGui::NewLine();
	}
	texts_order.clear();
	for (std::pair<std::string, std::tuple<float*, float, float>> p : float_sliders) {
        float* ptr = std::get<0>(p.second);
        float min = std::get<1>(p.second);
        float max = std::get<2>(p.second);
		ImGui::SliderFloat(p.first.c_str(), ptr, min, max);
	}

	for (std::pair<std::string, std::tuple<glm::vec3*, float, float>> p : float_3sliders) {
        glm::vec3* ptr = std::get<0>(p.second);
        float min = std::get<1>(p.second);
        float max = std::get<2>(p.second);
		ImGui::SliderFloat3(p.first.c_str(), &ptr->x, min, max);
	}

	for (std::pair<std::string, std::tuple<int*, int, int>> p : integer_sliders) {
		std::string& title = p.first;
		int* ptr = std::get<0>(p.second);
		int min = std::get<1>(p.second);
		int max = std::get<2>(p.second);
		ImGui::SliderInt(title.c_str(), ptr, min, max);
	}
	for (std::pair<std::string, glm::vec3*> p : color_pickers) {
		std::string& title = p.first;
		glm::vec3* v = p.second;
		ImGui::ColorEdit3(title.c_str(), (float*)v);
	}
    for (auto t : drags_3floats) {
        glm::vec3* ptr = std::get<0>(t.second);
        float& min = std::get<1>(t.second);
        float& max = std::get<2>(t.second);
        float& speed = std::get<3>(t.second);
        ImGui::DragFloat3(t.first.c_str(), &ptr->x, speed, min, max);
    }
    for (auto t : drags_2floats) {
        glm::vec2* ptr = std::get<0>(t.second);
        float& min = std::get<1>(t.second);
        float& max = std::get<2>(t.second);
        float& speed = std::get<3>(t.second);
        ImGui::DragFloat2(t.first.c_str(), &ptr->x, speed, min, max);
    }
    for (auto t : drags_1floats) {
        float* ptr = std::get<0>(t.second);
        float& min = std::get<1>(t.second);
        float& max = std::get<2>(t.second);
        float& speed = std::get<3>(t.second);
        ImGui::DragFloat(t.first.c_str(), ptr, speed, min, max);
    }
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	bool clicked = ImGui::Button("save scrrenshot");
	if (clicked) {
		srand((unsigned int)time(0));
		SaveTo(std::string("screenshots/") + std::to_string(rand()) + ".png");
	}
	ImGui::End();
}

void DebugTest::AddText(std::string title, std::string value) {
	texts_order.push_back(title);
	texts[title] = value;
}

void DebugTest::AddFloatSlider(std::string title, float* ptr, float min, float max) {
	float_sliders[title] = std::make_tuple(ptr, min, max);
}

void DebugTest::AddInteger(std::string title, int* ptr, int min, int max) {
	integer_sliders[title] = std::make_tuple(ptr, min, max);
}

void DebugTest::Add3FloatSlider(std::string title, glm::vec3* offset, float min, float max) {
	float_3sliders[title] = std::make_tuple(offset, min, max);
}

void DebugTest::AddColorPicker(std::string title, glm::vec3* c) {
	color_pickers[title] = c;
}

void DebugTest::Add3FloatsDrag(std::string title, glm::vec3* ptr, float min, float max, float speed) {
    this->drags_3floats[title] = std::make_tuple(ptr, min, max, speed);
}

void DebugTest::Add2FloatsDrag(std::string title, glm::vec2* ptr, float min, float max, float speed) {
    this->drags_2floats[title] = std::make_tuple(ptr, min, max, speed);
}

void DebugTest::Add1FloatsDrag(std::string title, float* ptr, float min, float max, float speed) {
    this->drags_1floats[title] = std::make_tuple(ptr, min, max, speed);
}

void DebugTest::Render() {
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

DebugTest::~DebugTest() { }
#include "Mesh.h"


Mesh::Mesh(std::string filePath) {
	vertices = nullptr;
	texCoords = nullptr;
	normals = nullptr;
	LoadFromFile(filePath);
}

void Mesh::Render(glm::mat4 model, glm::mat4 view, glm::mat4 proj, VertexArray& vao, ShaderProgram& shader) {
    shader.FillUniformMat4f("u_model", model);
	shader.FillUniformMat4f("u_view", view);
	shader.FillUniformMat4f("u_proj", proj);
	//UpdateShaderClipPlane(&shader);

	float* vertices = this->getVerticesData();
	float* texCoords = this->getTexCoordsData();
	float* normals = this->getNormalsData();
	
	unsigned int verticesCount =  this->getVerticeCount();
    float* data = new float[8 * verticesCount];
    for (unsigned int i = 0; i < verticesCount; ++i) {
        data[8 * i] = vertices[3 * i];
        data[8 * i + 1] = vertices[3 * i + 1];
        data[8 * i + 2] = vertices[3 * i + 2];
        data[8 * i + 3] = texCoords[2 * i];
        data[8 * i + 4] = texCoords[2 * i + 1];
        data[8 * i + 5] = normals[3 * i];
        data[8 * i + 6] = normals[3 * i + 1];
        data[8 * i + 7] = normals[3 * i + 2];
    }
    VertexBuffer vbo(data, 8 * verticesCount * sizeof(float));
    delete data;
    BufferLayout layout;
    layout.AddElement<float>(3, 0);
    layout.AddElement<float>(2, 1);
    layout.AddElement<float>(3, 2);

	/*VertexBuffer vbo1(vertices, 3 * verticesCount * sizeof(float));
	BufferLayout layout1;
	layout1.AddElement<float>(3, 0);

	VertexBuffer vbo2(texCoords, 2 * verticesCount * sizeof(float));
	BufferLayout layout2;
	layout2.AddElement<float>(2, 1);

	VertexBuffer vbo3(normals, 3 * verticesCount * sizeof(float));
	BufferLayout layout3;
	layout3.AddElement<float>(3, 2);*/

	vao.AddBuffer(vbo, layout);
	//vao.AddBuffer(vbo2, layout2);
	//vao.AddBuffer(vbo3, layout3);

    shader.Bind();
    vao.Bind();
	glCall(glDrawArrays(GL_TRIANGLES, 0, vertices_count));
	vao.Unbind();
	shader.Unbind();
}

void Mesh::LoadFromFile(std::string filePath) {
	std::vector<std::tuple<float, float, float>> tmp_vertices;
	std::vector<std::tuple<float, float, float>> tmp_texCoords;
	std::vector<std::tuple<float, float, float>> tmp_normals;
	std::vector<int> vertices_indexes;
	std::vector<int> texCoords_indexes;
	std::vector<int> normals_indexes;
	std::ifstream input(filePath);
	std::string line;
	while (std::getline(input, line)) {
		if (line == "") continue;
		std::vector<std::string> v = SplitLine(line);
		if (v[0][0] == '#') {
			// comment
		}
		else if (v[0] == "v") {
			// vertices coordinates
			std::tuple<float, float, float> coords(0.0f, 0.0f, 0.0f);
			std::get<0>(coords) = stof(v[1]);
			std::get<1>(coords) = stof(v[2]);
			std::get<2>(coords) = stof(v[3]);
			//if (v.size() == 5) std::get<3>(coords) = stof(v[4]);
			tmp_vertices.push_back(coords);
		}
		else if (v[0] == "vt") {
			// vertice textures coordinates
			std::tuple<float, float, float> coords(0.0f, 0.0f, 0.0f);
			std::get<0>(coords) = stof(v[1]);
			if (v.size() >= 3) std::get<1>(coords) = stof(v[2]);
			if (v.size() == 4) std::get<2>(coords) = stof(v[3]);
			tmp_texCoords.push_back(coords);
		}
		else if (v[0] == "vn") {
			// vertices normals
			std::tuple<float, float, float> coords(0.0f, 0.0f, 0.0f);
			std::get<0>(coords) = stof(v[1]);
			std::get<1>(coords) = stof(v[2]);
			std::get<2>(coords) = stof(v[3]);
			tmp_normals.push_back(coords);
		}
		else if (v[0] == "vp") {
			// vertices parameters
			/*std::tuple<float, float, float> coords(0.0f, 0.0f, 0.0f);
			std::get<0>(coords) = stof(v[1]);
			if (v.size() >= 3) std::get<1>(coords) = stof(v[2]);
			if (v.size() == 4) std::get<1>(coords) = stof(v[3]);
			tmp_normals.push_back(coords);*/
		}
		else if (v[0] == "f") {
			// faces
			std::vector<std::tuple<int, int, int>> face;
			for (int i = 1; i < 4; ++i) {
				std::vector<std::string> vertice = SplitLine(v[i], '/');
				vertices_indexes.push_back(stoi(vertice[0]));
				texCoords_indexes.push_back(vertice[1] == "" ? -1 : stoi(vertice[1]));
				normals_indexes.push_back(stoi(vertice[2]));
			}
		}
		else if (v[0] == "l") {
			// line
		}
	}

	vertices_count = vertices_indexes.size();
	vertices = new float[3 * vertices_count];
	texCoords = new float[2 * vertices_count];
	normals = new float[3 * vertices_count];
	for (unsigned int i = 0; i < vertices_indexes.size(); ++i) {
		int vertice_index = vertices_indexes[i] - 1;
		int tex_index = texCoords_indexes[i] - 1;
		int normal_index = normals_indexes[i] - 1;

		vertices[3 * i] = std::get<0>(tmp_vertices[vertice_index]);
		vertices[3 * i + 1] = std::get<1>(tmp_vertices[vertice_index]);
		vertices[3 * i + 2] = std::get<2>(tmp_vertices[vertice_index]);
		if (tex_index >= 0) {
			texCoords[2 * i] = std::get<0>(tmp_texCoords[tex_index]);
			texCoords[2 * i + 1] = std::get<1>(tmp_texCoords[tex_index]);
		}
		else texCoords[2 * i] = texCoords[2 * i + 1] = 0.0f;
		normals[3 * i] = std::get<0>(tmp_normals[normal_index]);
		normals[3 * i + 1] = std::get<1>(tmp_normals[normal_index]);
		normals[3 * i + 2] = std::get<2>(tmp_normals[normal_index]);
	}
}

std::vector<std::string> Mesh::SplitLine(std::string line, char sep) {
	std::vector<std::string> res;
	std::string current = "";
	for (char c : line) {
		if (c == sep) {
			res.push_back(current);
			current.clear();
			continue;
		}
		current.push_back(c);
	}
	if (current != "") res.push_back(current);
	return res;
}

unsigned int Mesh::getVerticeCount() const {
	return this->vertices_count;
}

float* Mesh::getVerticesData() const {
	return vertices;
}

float* Mesh::getTexCoordsData() const {
	return texCoords;
}

float* Mesh::getNormalsData() const {
	return normals;
}

Mesh::~Mesh() {
	if (vertices != nullptr) delete[] vertices;
	if (texCoords != nullptr) delete[] texCoords;
	if (normals != nullptr) delete[] normals;
}
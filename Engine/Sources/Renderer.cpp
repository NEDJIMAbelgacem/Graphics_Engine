#include "Renderer.h"

#define PI 3.14159f

Renderer::Renderer() {
	glCall(glEnable(GL_MULTISAMPLE));
	glCall(glEnable(GL_BLEND));
	glCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	glCall(glEnable(GL_DEPTH_TEST));
	//glCall(glEnable(GL_CULL_FACE));
	//glCall(glCullFace(GL_BACK));
	//glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
}

void Renderer::SetClipPlane(glm::vec4 plane) {
	this->clip_plane = plane;
}

void Renderer::EnableClipping() {
	this->clip_enabled = true;
}

void Renderer::DisableClipping() {
	this->clip_enabled = false;
}

void Renderer::UpdateShaderClipPlane(ShaderProgram* shader) {
	shader->FillUniform1i("u_isClipEnabled", this->clip_enabled ? 1 : 0);
	shader->FillUniform4f("u_clipPlane", clip_plane[0], clip_plane[1], clip_plane[2], clip_plane[3]);
}

void Renderer::UpdateShaderMatrices(ShaderProgram* shader) {
	shader->FillUniformMat4f("u_view", view_matrix);
	shader->FillUniformMat4f("u_proj", proj_matrix);
}
/*
void Renderer::DrawWaterPlane(WaterPlane& wp, VertexArray& vao) {
	ShaderProgram* shader = wp.GetShader();
	VertexBuffer* vbo = wp.GetVertexBuffer();
	IndexBuffer* ibo = wp.GetIndexBuffer();
	FrameBuffer* above = wp.GetAboveFrameBuffer();
	FrameBuffer* below = wp.GetBelowFrameBuffer();
	Texture* dudv_map = wp.GetDuDvMap();
	Texture* normal_map = wp.GetNormalMap();

	wp.MoveWater();
	UpdateShaderClipPlane(shader);
	UpdateShaderMatrices(shader);
	shader->FillUniform1i("u_aboveTex", 0);
	shader->FillUniform1i("u_belowTex", 1);
	shader->FillUniform1i("u_dudvTex", 2);
	shader->FillUniform1i("u_normalTex", 3);
	shader->FillUniform1i("u_depthTex", 4);
	above->BindTexture(0);
	below->BindTexture(1);
	dudv_map->Bind(2);
	normal_map->Bind(3);
	below->BindDepthTexture(4);

	BufferLayout layout;
	layout.AddElement<float>(3, 0);
	layout.AddElement<float>(2, 1);
	vao.AddBuffer(*vbo, layout);

	DrawIndexes(vao, *ibo, *shader, GL_TRIANGLES);

	below->UnbindTexture();
	below->UnbindDepthTexture();
	above->UnbindTexture();
}**/

/*void Renderer::DrawBatch(Batch& batch, VertexArray& vao) {
	VertexBuffer* vbo = batch.GetOffsetsBuffer();
	Model* model = batch.GetModel();
	int instances_count = batch.GetInstancesCount();
	ShaderProgram* shader = model->GetShader();
	//Texture* tex = model->GetGlobalTexture();
	shader->FillUniform1i("u_isBatchRender", 1);
	UpdateShaderClipPlane(shader);
	UpdateShaderMatrices(shader);

	BufferLayout layout;
	layout.AddElement<float>(3, 3);
	vao.AddInstanceBuffer(*vbo, layout, 1);

	/*if (tex != nullptr) {
		shader->FillUniform1i("u_material.diffuseMaps[0]", 0);
		tex->Bind(0);
	}*/

	/*for (int i = 0; i < model->GetComponentsCount(); ++i) {
		ModelComponent* comp = model->GetComponent(i);
		DrawModelComponentInstanced(instances_count, comp, vao, *shader);
	}*/
	//tex->Unbind();
//}

void Renderer::DrawTerrain(Terrain& terrain, VertexArray& vao) {
	VertexBuffer* vertices = terrain.GetVertexBuffer();
	IndexBuffer* indexes = terrain.GetIndexBuffer();
	std::vector<Texture*> textures = terrain.GetTextures();
	HeightMap* height_map = terrain.GetHeightMap();
	ShaderProgram* shader = terrain.GetShader();

	UpdateShaderClipPlane(shader);
	UpdateShaderMatrices(shader);

	BufferLayout layout;
	layout.AddElement<float>(3, 0);
	layout.AddElement<float>(2, 1);
	vao.AddBuffer(*vertices, layout);

	int instances_count = terrain.GetInstancesCount();
	height_map->Bind(0);
	for (int i = 0; i < textures.size(); ++i) textures[i]->Bind(1 + i);

	DrawInstancedIndexes(instances_count, vao, *indexes, *shader, GL_TRIANGLES);
	for (Texture* t : textures) t->Unbind();
	height_map->Unbind();
}

/*void Renderer::DrawModel(Model& object, VertexArray& vao) {
	ShaderProgram* shader = object.GetShader();
	//Texture* tex = object.GetGlobalTexture();
	UpdateShaderClipPlane(shader);
	if (tex != nullptr) {
		shader->FillUniform1i("u_material.materialTexture", 0);
		tex->Bind(0);
	}
	this->UpdateShaderMatrices(shader);
	for (int i = 0; i < object.GetComponentsCount(); ++i) {
		ModelComponent* comp = object.GetComponent(i);
		DrawModelComponent(comp, vao, *shader);
	}
	if (tex != nullptr) tex->Unbind();
}*/

void Renderer::SetMatrices(glm::mat4 view, glm::mat4 proj) {
	view_matrix = view;
	proj_matrix = proj;
}

void Renderer::DrawSkyBox(SkyBox& skybox, VertexArray& vao) {
	ShaderProgram* shader = skybox.GetShader();
	glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(view_matrix, scale, rotation, translation, skew, perspective);
	//glm::mat4 m = glm::rotate(skybox.GetModelMatrix(), 0.05f * skybox.GetRotationSpeed(), glm::vec3(0.0f, 1.0f, 0.0f));
	//skybox.SetModelMatrix(m);
	//shader->FillUniformMat4f("u_model", m);
	shader->FillUniformMat4f("u_view", glm::toMat4(rotation));
	shader->FillUniformMat4f("u_proj", proj_matrix);
	shader->FillUniform1i("u_skybox", 0);

	float skybox_vertices[] = {
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,
		 
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};
	/*for (int i = 0; i < 36; ++i) {
		for (int j = 0; j < 3; ++j) {
			skybox_vertices[3 * i + j] *= 1000.0f;
		}
	}*/

	VertexBuffer vbo(skybox_vertices, sizeof(skybox_vertices));
	BufferLayout layout;
	layout.AddElement<float>(3, 0);

	vao.AddBuffer(vbo, layout);
	glCall(glDepthMask(GL_FALSE));
	skybox.Bind(0);
	DrawArrays(36, vao, *shader);
	glCall(glDepthMask(GL_TRUE));
}

/*void Renderer::DrawModelComponent(ModelComponent* component, VertexArray& vao, ShaderProgram& shader) {
	bool has_texture = component->GetUVcount() > 0; 
	VertexBuffer* vertices = component->GetVerticesBuffer();
	VertexBuffer* texCoords = nullptr;
	if (has_texture)
		texCoords = component->GetTexCoordsBuffer(0);
	VertexBuffer* normals = component->GetNormalsBuffer();
	IndexBuffer* ibo = component->GetIndexesBuffer();

	unsigned int verticesCount = component->GetVerticesCount();

	BufferLayout indexes_layout;
	indexes_layout.AddElement<float>(3, 0);

	BufferLayout texCoords_layout;
	if (has_texture)
		texCoords_layout.AddElement<float>(component->GetUVComponentsCount(0), 1);

	BufferLayout normals_layout;
	normals_layout.AddElement<float>(3, 2);

	vao.AddBuffer(*vertices, indexes_layout);
	if (has_texture)
		vao.AddBuffer(*texCoords, texCoords_layout);
	vao.AddBuffer(*normals, normals_layout);

	component->BindTextures(shader);
	DrawIndexes(vao, *ibo, shader);
	component->UnbindTextures();
}*/

void Renderer::DrawModelComponentInstanced(int instances_count, ModelComponent* component, VertexArray& vao, ShaderProgram& shader) {
	bool has_texCoords = component->GetUVcount() > 0;
	VertexBuffer* vertices = component->GetVerticesBuffer();
	VertexBuffer* texCoords = nullptr;
	if (has_texCoords) 
		texCoords = component->GetTexCoordsBuffer(0);
	VertexBuffer* normals = component->GetNormalsBuffer();
	IndexBuffer* ibo = component->GetIndexesBuffer();

	unsigned int verticesCount = component->GetVerticesCount();

	BufferLayout indexes_layout;
	indexes_layout.AddElement<float>(3, 0);
	
	BufferLayout texCoords_layout;
	if (has_texCoords)
		texCoords_layout.AddElement<float>(component->GetUVComponentsCount(0), 1);

	BufferLayout normals_layout;
	normals_layout.AddElement<float>(3, 2);

	vao.AddBuffer(*vertices, indexes_layout);
	if (has_texCoords)
		vao.AddBuffer(*texCoords, texCoords_layout);
	vao.AddBuffer(*normals, normals_layout);

	component->BindTextures(shader);
	DrawInstancedIndexes(instances_count, vao, *ibo, shader);
	component->UnbindTextures();
}

void Renderer::DrawPlaneGrid(Grid& grid, VertexArray& vao, glm::vec3 lines_color) {
	ShaderProgram* shader = grid.GetShader();
	VertexBuffer* vbo = grid.GetVertexBuffer();
	IndexBuffer* ibo = grid.GetIndexBuffer();

	UpdateShaderClipPlane(shader);
	shader->FillUniformMat4f("u_view", view_matrix);
	shader->FillUniformMat4f("u_proj", proj_matrix);
	shader->FillUniform3f("u_line_color", lines_color.r, lines_color.g, lines_color.b);
	BufferLayout layout;
	layout.AddElement<float>(3, 0);
	vao.AddBuffer(*vbo, layout);

	DrawIndexes(vao, *ibo, *shader, GL_LINES);
}

void Renderer::DrawMesh(glm::mat4 model, glm::mat4 view, glm::mat4 proj, const Mesh& m, VertexArray& vao, ShaderProgram& shader) {
	shader.FillUniformMat4f("u_model", model);
	shader.FillUniformMat4f("u_view", view);
	shader.FillUniformMat4f("u_proj", proj);
	UpdateShaderClipPlane(&shader);

	float* vertices = m.getVerticesData();
	float* texCoords = m.getTexCoordsData();
	float* normals = m.getNormalsData();
	
	unsigned int verticesCount = m.getVerticeCount();

	VertexBuffer vbo1(vertices, 3 * verticesCount * sizeof(float));
	BufferLayout layout1;
	layout1.AddElement<float>(3, 0);

	VertexBuffer vbo2(texCoords, 2 * verticesCount * sizeof(float));
	BufferLayout layout2;
	layout2.AddElement<float>(3, 1);

	VertexBuffer vbo3(normals, 3 * verticesCount * sizeof(float));
	BufferLayout layout3;
	layout3.AddElement<float>(3, 2);

	vao.AddBuffer(vbo1, layout1);
	vao.AddBuffer(vbo2, layout2);
	vao.AddBuffer(vbo3, layout3);

	DrawArrays(verticesCount, vao, shader);
}

void Renderer::DrawCircle(std::pair<float, float> pos, float r, int triangle_count, std::tuple<float, float, float> color, VertexArray& vao, ShaderProgram& shader) {
	float red = std::get<0>(color);
	float green = std::get<1>(color);
	float blue = std::get<2>(color);
	float cx = pos.first, cy = pos.second;
	float* vertices = (float*)alloca(5 * (triangle_count + 1) * sizeof(float));
	unsigned int* indexes = (unsigned int*)alloca(3 * triangle_count * sizeof(unsigned int));
	vertices[5 * triangle_count] = cx;
	vertices[5 * triangle_count + 1] = cy;
	vertices[5 * triangle_count + 2] = 0.0f;
	vertices[5 * triangle_count + 3] = 0.0f;
	vertices[5 * triangle_count + 4] = 1.0f;
	for (int i = 0; i < triangle_count; ++i) {
		float angle = (float)i * 2.0f * PI / triangle_count;
		float x = cx + r * cos(angle);
		float y = cy + r * sin(angle);
		vertices[5 * i] = x;
		vertices[5 * i + 1] = y;
		if (i % 2 == 0) {
			vertices[5 * i + 2] = 1.0f;
			vertices[5 * i + 3] = 0.0f;
			vertices[5 * i + 4] = 0.0f;
		}
		else {
			vertices[5 * i + 2] = 0.0f;
			vertices[5 * i + 3] = 1.0f;
			vertices[5 * i + 4] = 0.0f;
		}
	}
	for (int i = 0; i < triangle_count; ++i) {
		indexes[3 * i] = triangle_count;
		indexes[3 * i + 1] = i;
		indexes[3 * i + 2] = (i + 1) % triangle_count;
	}

	VertexBuffer vbo(vertices, 5 * (triangle_count + 1) * sizeof(float));
	IndexBuffer ibo(indexes, 3 * triangle_count);
	BufferLayout layout;
	layout.AddElement<float>(2, 0);
	layout.AddElement<float>(3, 1);
	vao.AddBuffer(vbo, layout);
	//shader->FillUniform3f("u_color", std::get<0>(color), std::get<1>(color), std::get<2>(color));
	DrawIndexes(vao, ibo, shader);
}
/*
void Renderer::DrawTriangle(std::vector<std::pair<float, float>> vertices, std::vector<std::tuple<float, float, float>> vertices_colors, VertexArray& vao, ShaderProgram& shader) {
	ASSERT(vertices.size() == 3);
	float* vertices_a = (float*)alloca(5 * vertices.size() * sizeof(float));
	for (int i = 0; i < 3; ++i) {
		vertices_a[5 * i] = vertices[i].first;
		vertices_a[5 * i + 1] = vertices[i].second;
		vertices_a[5 * i + 2] = std::get<0>(vertices_colors[i]);
		vertices_a[5 * i + 3] = std::get<1>(vertices_colors[i]);
		vertices_a[5 * i + 4] = std::get<2>(vertices_colors[i]);
	}
	unsigned int indexes[3] = { 0, 1, 2 };
	VertexBuffer vbo(vertices_a, 5 * vertices.size() * sizeof(float));
	IndexBuffer ibo(indexes, 3);
	BufferLayout layout;
	layout.AddElement<float>(2);
	layout.AddElement<float>(3);
	vao.AddBuffer(vbo, layout);
	Draw(vao, ibo, shader);
}
*/
std::pair<float, float> Renderer::rotate(std::pair<float, float> pos, float angle) {
	float& x = pos.first, y = pos.second;
	float cos_angle = cos(angle);
	float sin_angle = sin(angle);
	return std::make_pair(cos_angle * x - sin_angle * y, sin_angle * x + cos_angle * y);
};

void Renderer::RotatingCircle(float cx, float cy, float r, VertexArray& vao, ShaderProgram& shader, unsigned int triangle_count) {
	static float r_angle = 0.0f;
	static float a_inc = 0.05f;
	float* vertices = (float*)alloca(5 * (triangle_count + 1) * sizeof(float));
	unsigned int* indexes = (unsigned int*)alloca(3 * triangle_count * sizeof(unsigned int));
	vertices[5 * triangle_count] = cx;
	vertices[5 * triangle_count + 1] = cy;
	vertices[5 * triangle_count + 2] = 1.0f;
	vertices[5 * triangle_count + 3] = 1.0f;
	vertices[5 * triangle_count + 4] = 1.0f;
	for (int i = 0; i < triangle_count; ++i) {
		float angle = (float)i * 2.0f * PI / triangle_count;
		float x = cx + r * cos(angle);
		float y = cy + r * sin(angle);
		std::pair<float, float> p = rotate(std::make_pair(x, y), r_angle);
		vertices[5 * i] = p.first;
		vertices[5 * i + 1] = p.second;
		if (i < 10) {
			vertices[5 * i + 2] = 1.0f;
			vertices[5 * i + 3] = 0.0f;
			vertices[5 * i + 4] = 0.0f;
		}
		else if (i < 20) {
			vertices[5 * i + 2] = 0.0f;
			vertices[5 * i + 3] = 1.0f;
			vertices[5 * i + 4] = 0.0f;
		}
		else {
			vertices[5 * i + 2] = 0.0f;
			vertices[5 * i + 3] = 0.0f;
			vertices[5 * i + 4] = 1.0f;
		}
	}
	for (int i = 0; i < triangle_count; ++i) {
		indexes[3 * i] = triangle_count;
		indexes[3 * i + 1] = i;
		indexes[3 * i + 2] = (i + 1) % triangle_count;
	}
	VertexBuffer vbo(vertices, 5 * triangle_count * sizeof(float));
	IndexBuffer ibo(indexes, 3 * triangle_count);
	BufferLayout layout;
	layout.AddElement<float>(2, 0);
	layout.AddElement<float>(3, 1);
	vao.AddBuffer(vbo, layout);
	DrawIndexes(vao, ibo, shader);
	if (r_angle >= 2 * PI) r_angle = 0.0f;
	r_angle += a_inc;
}

void Renderer::DrawCube(ColoredCube& cube, VertexArray& vao) {
	ShaderProgram* shader = cube.GetShader();
	VertexBuffer* vbo = cube.GetVertexBuffer();
	IndexBuffer* ibo = cube.GetIndexBuffer();

	UpdateShaderClipPlane(shader);
	shader->FillUniformMat4f("u_view", view_matrix);
	shader->FillUniformMat4f("u_proj", proj_matrix);

	BufferLayout layout;
	layout.AddElement<float>(3, 0);
	vao.AddBuffer(*vbo, layout);

	this->DrawIndexes(vao, *ibo, *shader);
}

void Renderer::DrawCube(glm::vec3 center, float radius, const glm::mat4& model, Texture& texture, VertexArray& vao, ShaderProgram& shader) {
	
	UpdateShaderClipPlane(&shader);
	shader.FillUniformMat4f("u_model", model);
	shader.FillUniformMat4f("u_view", view_matrix);
	shader.FillUniformMat4f("u_proj", proj_matrix);
	
	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

	for (int i = 0; i < 36; ++i) {
		for (int j = 0; j < 6; ++j) {
			vertices[8 * i + j] = center[j % 3] + vertices[8 * i + j] * radius;
		}
	}

	VertexBuffer vbo(vertices, sizeof(vertices));
	BufferLayout layout;
	layout.AddElement<float>(3, 0);
	layout.AddElement<float>(3, 2);
	layout.AddElement<float>(2, 1);

	vao.AddBuffer(vbo, layout);

	texture.Bind(0);
	shader.FillUniform1i("u_tex", 0);

	DrawArrays(36, vao, shader);
}

void Renderer::DrawPlane(Plane& plane, VertexArray& vao) {
	ShaderProgram* shader = plane.GetShader();
	VertexBuffer* vbo = plane.GetVertexBuffer();

	BufferLayout layout;
	layout.AddElement<float>(3, 0);
	vao.AddBuffer(*vbo, layout);

	UpdateShaderClipPlane(shader);
	shader->FillUniformMat4f("u_view", view_matrix);
	shader->FillUniformMat4f("u_proj", proj_matrix);

	DrawArrays(4, vao, *shader, GL_TRIANGLE_FAN);
}

void Renderer::DrawQuad(Quad& quad, VertexArray& vao) {
	ShaderProgram* shader = quad.GetShader();
	Texture* diffuseMap = quad.GetDiffuseMap();
	Texture* specularMap = quad.GetSpecularMap();
	VertexBuffer* vbo = quad.GetVertexBuffer();
	IndexBuffer* ibo = quad.GetIndexBuffer();

	BufferLayout layout;
	layout.AddElement<float>(3, 0);
	layout.AddElement<float>(2, 1);
	vao.AddBuffer(*vbo, layout);

	UpdateShaderClipPlane(shader);
	shader->FillUniformMat4f("u_view", view_matrix);
	shader->FillUniformMat4f("u_proj", proj_matrix);
	shader->FillUniform1i("u_material.diffuseMap", 0);
	shader->FillUniform1i("u_material.specularMap", 1);

	diffuseMap->Bind(0);
	specularMap->Bind(1);
	DrawIndexes(vao, *ibo, *shader);
	diffuseMap->Unbind();
	specularMap->Unbind();
}

void Renderer::DrawIndexes(const VertexArray& va, const IndexBuffer& ib, const ShaderProgram& shader, GLenum type) const {
	shader.Bind();
	va.Bind();
	ib.Bind();
	glCall(glDrawElements(type, ib.GetCount(), GL_UNSIGNED_INT, 0));
	ib.Unbind();
	va.Unbind();
	shader.Unbind();
}

void Renderer::DrawArrays(unsigned int vertices_count, const VertexArray& va, const ShaderProgram& shader, GLenum type) const {
	shader.Bind();
	va.Bind();
	glCall(glDrawArrays(type, 0, vertices_count));
	va.Unbind();
	shader.Unbind();
}

void Renderer::DrawInstancedArrays(unsigned int vertices_count, unsigned int instances_count, const VertexArray& va, const ShaderProgram& shader, GLenum type) const {
	shader.Bind();
	va.Bind();
	glCall(glDrawArraysInstanced(type, 0, vertices_count, instances_count));
	va.Unbind();
	shader.Unbind();
}

void Renderer::DrawInstancedIndexes(unsigned int instances_count, const VertexArray& va, const IndexBuffer& ib, const ShaderProgram& shader, GLenum type) const {
	shader.Bind();
	va.Bind();
	ib.Bind();
	glCall(glDrawElementsInstanced(type, ib.GetCount(), GL_UNSIGNED_INT, 0, instances_count));
	ib.Unbind();
	va.Unbind();
	shader.Unbind();
}

void Renderer::ClearScr() const {
	glCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}
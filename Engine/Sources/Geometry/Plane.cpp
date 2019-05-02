#include "Geometry/Plane.h"

Plane::Plane(glm::vec4 equation, glm::vec3 center, float size, ShaderProgram* shader)
	: equation(equation), size(size) {
    vao = new VertexArray();
	this->shader = shader;
	this->SetLightColor(glm::vec3(1.0f));
    this->SetLightPosition(glm::vec3(0.0f, 100.0f, 100.0f));
    this->SetSurfaceParameters(0.0f, 1.0f, 1.0f);
    this->SetCenterOffset(center);

	float vertices[3 * 4];
    float normals[3 * 4];

	float p = size / 2.0f;
	for (int i = 0; i < 4; ++i) {
		float theta = (0.25f + 0.5f * i) * glm::pi<float>();
		float xp, yp, zp;
		if (equation[0] != 0) {
			yp = p * cos(theta) + center.y;
			zp = p * sin(theta) + center.z;
			xp = (equation[3] - yp * equation[1] - zp * equation[2]) / equation[0];
		}
		else if (equation[1] != 0) {
			xp = p * cos(theta) + center.x;
			zp = p * sin(theta) + center.z;
			yp = (equation[3] - xp * equation[0] - zp * equation[2]) / equation[1];
		}
		else {
			xp = p * cos(theta) + center.x;
			yp = p * sin(theta) + center.y;
			zp = (equation[3] - xp * equation[0] - yp * equation[1]) / equation[2];
		}
		
		vertices[i * 3] = xp;
		vertices[i * 3 + 1] = yp;
		vertices[i * 3 + 2] = zp;
	}

	vertices_vbo = new VertexBuffer(vertices, 4 * 3 * sizeof(float));
    BufferLayout vertices_layout;
    vertices_layout.AddElement<float>(3, vertices_location);
    vao->AddBuffer(*vertices_vbo, vertices_layout);

    for (int i = 0; i < 3 * 4; ++i) normals[i] = equation[i % 3];
    normals_vbo = new VertexBuffer(normals, 4 * 3 * sizeof(float));
    BufferLayout normals_layout;
    normals_layout.AddElement<float>(3, normals_location);
    vao->AddBuffer(*normals_vbo, normals_layout);

    //unsigned int indexes[]{ 0, 1, 2, 3 };
    //this->ibo = new IndexBuffer(indexes, 4);
    //delete indexes;
}

Plane::~Plane() {
	delete vertices_vbo;
    delete normals_vbo;
    delete vao;
}

void Plane::Render() {
    shader->Bind();
    vao->Bind();
    glCall(glDisable(GL_CULL_FACE));
	glCall(glDrawArrays(GL_TRIANGLE_FAN, 0, 4));
    glCall(glEnable(GL_CULL_FACE));
    vao->Unbind();
	shader->Unbind();
}

void Plane::SetModelMatrix(glm::mat4 matrix) {
    this->model_matrix = matrix;
    this->shader->FillUniformMat4f("u_model", matrix);
}

void Plane::SetCenterOffset(glm::vec3 center) {
    glm::mat4 m = glm::identity<glm::mat4>();
    m = glm::translate(m, center);
    this->SetModelMatrix(m);
}
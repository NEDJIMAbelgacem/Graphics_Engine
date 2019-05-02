#pragma once
#include "Core/Common.h"
#include "Core/VertexBuffer.h"
#include "Core/ShaderProgram.h"
#include "Core/VertexArray.h"
#include "Core/IndexBuffer.h"

#include "ShaderComponents/PointLight.h"
#include "ShaderComponents/SurfaceParameters.h"
#include "AbstractClasses/Renderable.h"

#define vertices_location 0
#define uv_coords_location 1
#define normals_location 2
#define batch_instance_offset 3
#define tangent_location 4
#define bitangent_location 5

class Plane : public Renderable, public SurfaceParameters, public PointLight {
private:
    VertexArray* vao;
    //IndeXbuffer* ibo;
	VertexBuffer* vertices_vbo;
    VertexBuffer* normals_vbo;

	glm::vec4 equation;
	glm::vec3 center;
	float size;
    glm::mat4 model_matrix;
public:
	Plane(glm::vec4 equation, glm::vec3 center, float size, ShaderProgram* shader);
    ~Plane();

    void Render();

    void SetModelMatrix(glm::mat4 matrix);
	void SetCenterOffset(glm::vec3 center);
};
#pragma once
#include "Core/Common.h"
#include "Core/VertexBuffer.h"
#include "Core/ShaderProgram.h"
#include "Core/VertexArray.h"
#include "Core/IndexBuffer.h"

#include "GameObject.h"
#include "Components/Lighting.h"
#include "Components/Surface.h"
#include "Components/Transform.h"

#define vertices_location 0
#define uv_coords_location 1
#define normals_location 2
#define batch_instance_offset 3
#define tangent_location 4
#define bitangent_location 5

class Plane : public GameObject {
private:
    VertexArray* vao;
    //IndeXbuffer* ibo;
	VertexBuffer* vertices_vbo;
    VertexBuffer* normals_vbo;

    Transform transform;
    Surface surface;
    Lighting lighting;
    
	glm::vec4 equation;
	glm::vec3 center;
	float size;
    // glm::mat4 model_matrix;
public:
	Plane(glm::vec4 equation, glm::vec3 center, float size, ShaderProgram* shader);
    ~Plane();

    void Render();

    inline Transform* GetTransformComponent() { return &this->transform; }
    inline Surface* GetSurfaceComponent() { return &this->surface; }
    inline Lighting* GetLightingComponent() { return &this->lighting; }

    // void SetModelMatrix(glm::mat4 matrix);
	// void SetCenterOffset(glm::vec3 center);
};
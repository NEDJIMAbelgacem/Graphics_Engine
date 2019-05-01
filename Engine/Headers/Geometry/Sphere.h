#pragma once
#include "Core/Common.h"
#include "Core/ShaderProgram.h"
#include "Core/VertexArray.h"
#include "Core/VertexBuffer.h"
#include "Core/IndexBuffer.h"

#include "ShaderComponents/SurfaceParameters.h"
#include "ShaderComponents/PointLight.h"
#include "AbstractClasses/Renderable.h"

#define DEFAULT_SPHERE_OBJ_PATH "Resources/Geometry/Polo_Ball_v1_L3.123ca154e7bb-8048-4914-bba6-6cce468b0cfd/Polo_Ball_v1_L3.123ca154e7bb-8048-4914-bba6-6cce468b0cfd/13644_Polo_Ball_v1_L3.obj"
#define vertices_location 0
#define uv_coords_location 1
#define normals_location 2
#define batch_instance_offset 3
#define tangent_location 4
#define bitangent_location 5

class Sphere : public Renderable, public SurfaceParameters, public PointLight {
private:
    VertexArray* vao;
    VertexBuffer* vertices_vbo;
    VertexBuffer* normals_vbo;
    IndexBuffer* ibo;

    float radius;
    glm::vec3 position;
    glm::mat4 model_matrix;
public:
    Sphere(ShaderProgram* shader, glm::vec3 position, float radius, std::string obj_path = DEFAULT_SPHERE_OBJ_PATH);
    ~Sphere();

    void SetPosition(glm::vec3 position);
    void SetRadius(float radius);
    void SetModelMatrix(glm::mat4 matrix);

    void Render();
};
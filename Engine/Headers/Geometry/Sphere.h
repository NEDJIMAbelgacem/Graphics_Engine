#pragma once
#include "Core/Common.h"
#include "Core/ShaderProgram.h"
#include "Core/VertexArray.h"
#include "Core/VertexBuffer.h"
#include "Core/IndexBuffer.h"

#include "ShaderComponents/SurfaceParameters.h"
#include "ShaderComponents/PointLight.h"
#include "AbstractClasses/Renderable.h"

#define vertices_location 0
#define uv_coords_location 1
#define normals_location 2
#define batch_instance_offset 3
#define tangent_location 4
#define bitangent_location 5

#define SPHERE_THETA_TILES_COUNT 30
#define SPHERE_PHI_TILES_COUNT 30

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
    Sphere(ShaderProgram* shader, glm::vec3 position, float radius);
    ~Sphere();

    void SetPosition(glm::vec3 position);
    void SetRadius(float radius);
    void SetModelMatrix(glm::mat4 matrix);

    bool ray_intersection(glm::vec3 origin, glm::vec3 ray, float& depth);
    float is_point_inside(glm::vec3 point);

    void Render();
};
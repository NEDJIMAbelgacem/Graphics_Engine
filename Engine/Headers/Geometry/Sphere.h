#pragma once
#include "Core/Common.h"
#include "Core/ShaderProgram.h"
#include "Core/VertexArray.h"
#include "Core/VertexBuffer.h"
#include "Core/IndexBuffer.h"

#include "GameObject.h"
#include "Components/Transform.h"
#include "Components/Surface.h"
#include "Components/Lighting.h"

#define vertices_location 0
#define uv_coords_location 1
#define normals_location 2
#define batch_instance_offset 3
#define tangent_location 4
#define bitangent_location 5

#define SPHERE_THETA_TILES_COUNT 30
#define SPHERE_PHI_TILES_COUNT 30

// TODO : handle modification in transform component so that it affects position and radius class members

class Sphere : public GameObject {
protected:
    VertexArray* vao;
    VertexBuffer* vertices_vbo;
    VertexBuffer* normals_vbo;
    IndexBuffer* ibo;

    float radius;
    glm::vec3 position;

    Transform transform;
    Surface surface;
    Lighting lighting;
public: 
    Sphere(ShaderProgram* shader, glm::vec3 position, float radius);
    ~Sphere();

    inline Transform* GetTransformComponent() { return &this->transform; }
    inline Surface* GetSurfaceComponent() { return &this->surface; }
    inline Lighting* GetLightingComponent() { return &this->lighting; }

    void SetPosition(glm::vec3 position);
    void SetRadius(float radius);

    bool ray_intersection(glm::vec3 origin, glm::vec3 ray, float& depth);
    float is_point_inside(glm::vec3 point);

    void Render();
};
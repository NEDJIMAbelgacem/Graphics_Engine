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

#include "Logger.h"
#include "ShaderComponents/SurfaceParameters.h"
#include "ShaderComponents/PointLight.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


#define DEFAULT_CUBE_OBJ_PATH "Resources/Geometry/cube.obj"
#define vertices_location 0
#define uv_coords_location 1
#define normals_location 2
#define batch_instance_offset 3
#define tangent_location 4
#define bitangent_location 5

class Cube : public GameObject {
private:
    VertexArray* vao;
    VertexBuffer* vertices_vbo;
    VertexBuffer* normals_vbo;
    IndexBuffer* ibo;
    
    Transform transform;
    Surface surface;
    Lighting lighting;
public:
    Cube(ShaderProgram* shader, glm::vec3 position, glm::vec3 size, std::string obj_path = DEFAULT_CUBE_OBJ_PATH);
    ~Cube();

    inline Transform* GetTransformComponent() { return &this->transform; }
    inline Surface* GetSurfaceComponent() { return &this->surface; }
    inline Lighting* GetLightingComponent() { return &this->lighting; }

    void Render();
};
#pragma once
#include "Core/Common.h"
#include "Core/ShaderProgram.h"
#include "Core/VertexArray.h"
#include "Core/VertexBuffer.h"
#include "Core/IndexBuffer.h"

#include "ShaderComponents/SurfaceParameters.h"
#include "ShaderComponents/PointLight.h"
#include "AbstractClasses/Renderable.h"

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

class Cube : public Renderable, public SurfaceParameters, public PointLight {
private:
    VertexArray* vao;
    VertexBuffer* vertices_vbo;
    VertexBuffer* normals_vbo;
    IndexBuffer* ibo;

    glm::vec3 size;
    glm::vec3 position;
    glm::mat4 model_matrix;
public:
    Cube(ShaderProgram* shader, glm::vec3 position, glm::vec3 size, std::string obj_path = DEFAULT_CUBE_OBJ_PATH);
    ~Cube();

    void SetPosition(glm::vec3 position);
    void SetSize(glm::vec3 size);
    void SetModelMatrix(glm::mat4 matrix);

    void Render();
};
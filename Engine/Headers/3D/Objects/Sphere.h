#pragma once
#include "Core/Common.h"
#include "Core/ShaderProgram.h"
#include "Utilities/Maths.h"

namespace N3D {

#define vertices_location 0
#define uv_coords_location 1
#define normals_location 2
#define batch_instance_offset 3
#define tangent_location 4
#define bitangent_location 5

class Sphere : public Object_3D {
protected:
    VertexArray* vao = nullptr;
    VertexBuffer* vertices_vbo = nullptr;
    VertexBuffer* normals_vbo = nullptr;
    IndexBuffer* ibo = nullptr;

    int theta_tiles_count, phi_tiles_count;
    float radius;
    glm::vec3 color;
public:
    Sphere(glm::vec3 _position, float _radius, glm::vec3 _color, glm::vec3 _rotation = glm::vec3(0.0f), int nb_triangles = 50) 
        : Object_3D(_position, _rotation), radius(_radius), color(_color), theta_tiles_count(20), phi_tiles_count(20) {
        this->vao = new VertexArray();

        int vertices_offset = 0;
        int indexes_offset = 0;
        float* vertices = new float[3 * SPHERE_PHI_TILES_COUNT * (SPHERE_THETA_TILES_COUNT + 1)];
        float* normals = new float[3 * SPHERE_PHI_TILES_COUNT * (SPHERE_THETA_TILES_COUNT + 1)];
        unsigned int* indexes = new unsigned int[6 * SPHERE_PHI_TILES_COUNT * SPHERE_THETA_TILES_COUNT];
        
        float theta_step = glm::pi<float>() / SPHERE_THETA_TILES_COUNT;
        float phi_step = 2.0f * glm::pi<float>() / SPHERE_PHI_TILES_COUNT;
        for (int theta_index = 0; theta_index <= SPHERE_THETA_TILES_COUNT; ++theta_index) {
            for (int phi_index = 0; phi_index < SPHERE_PHI_TILES_COUNT; ++phi_index) {
                float theta = theta_index * theta_step;
                float phi = phi_index * phi_step;
                float normal_x = glm::sin(theta) * glm::cos(phi);
                float normal_y = glm::sin(theta) * glm::sin(phi);
                float normal_z = glm::cos(theta);

                normals[vertices_offset] = normal_x;
                normals[vertices_offset + 1] = normal_y;
                normals[vertices_offset + 2] = normal_z;
                vertices[vertices_offset] = normal_x;
                vertices[vertices_offset + 1] = normal_y;
                vertices[vertices_offset + 2] = normal_z;
                vertices_offset += 3;
                if (theta_index == SPHERE_THETA_TILES_COUNT) continue;

                int index1 = theta_index * SPHERE_PHI_TILES_COUNT + phi_index;
                int index2 = (theta_index + 1) * SPHERE_PHI_TILES_COUNT + phi_index;
                int index3 = (theta_index + 1) * SPHERE_PHI_TILES_COUNT + (phi_index + 1) % SPHERE_PHI_TILES_COUNT;
                int index4 = theta_index * SPHERE_PHI_TILES_COUNT + (phi_index + 1) % SPHERE_PHI_TILES_COUNT;
                indexes[indexes_offset] = index1;
                indexes[indexes_offset + 1] = index2;
                indexes[indexes_offset + 2] = index3;
                indexes[indexes_offset + 3] = index3;
                indexes[indexes_offset + 4] = index4;
                indexes[indexes_offset + 5] = index1;
                indexes_offset += 6;
            }
        }

        vertices_vbo = new VertexBuffer(vertices, vertices_offset * sizeof(float));
        BufferLayout vertices_layout;
        vertices_layout.AddElement<float>(3, vertices_location);
        vao->AddBuffer(*vertices_vbo, vertices_layout);
        delete [] vertices;

        normals_vbo = new VertexBuffer(normals, vertices_offset * sizeof(float));
        BufferLayout normals_layout;
        normals_layout.AddElement<float>(3, normals_location);
        vao->AddBuffer(*normals_vbo, normals_layout);
        delete [] normals;

        this->ibo = new IndexBuffer(indexes, indexes_offset);
        delete [] indexes;
    }

    ~Sphere() {
        delete vao;
        delete ibo;
        delete vertices_vbo;
    }

    glm::mat4 GetModelMatrix() {
        glm::mat4 scale = glm::scale(glm::identity<glm::mat4>(), glm::vec3(radius, radius, radius));
        glm::mat4 rotation_m = glm::rotate(glm::identity<glm::mat4>(), rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        rotation_m = glm::rotate(rotation_m, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        rotation_m = glm::rotate(rotation_m, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 translation = glm::translate(glm::identity<glm::mat4>(), glm::vec3(position.x, position.y, position.z));
        return translation * rotation_m * scale;
    }

    void FillShader(ShaderProgram& prg) {
        glm::mat4 model_m = this->GetModelMatrix();
        prg.FillUniformMat4f("u_model", model_m);
        prg.FillUniformVec3("u_color", this->color);
        prg.FillUniform1i("u_texture_is_used", 0);
    }

    void Render() override {
        vao->Bind();
        ibo->Bind();
        // glCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
        glCall(glDrawElements(GL_TRIANGLES, ibo->GetCount(), GL_UNSIGNED_INT, 0));
        // glCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
        vao->Unbind();
    }
};

}
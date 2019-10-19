#pragma once
#include "Core/Common.h"
#include "Core/ShaderProgram.h"
#include "Utilities/Maths.h"
#include "Core/UniformBuffer.h"

namespace N3D {

#define vertices_location 0
#define uv_coords_location 1
#define normals_location 2
#define batch_instance_offset 3
#define tangent_location 4
#define bitangent_location 5

class ColorfulIcosphere : public Object_3D {
protected:
    VertexArray* vao = nullptr;
    VertexBuffer* vertices_vbo = nullptr;
    VertexBuffer* normals_vbo = nullptr;
    IndexBuffer* ibo = nullptr;
    UniformBuffer* colors_ubo = nullptr;
    std::vector<int> colors_buffer;

    float radius;
    glm::vec3 color;
protected:
    std::vector<glm::vec3> vertices;
    // TODO : add normals
    std::vector<glm::vec3> normals;
    std::vector<int> faces;
protected:
    int FindMiddle(int v1, int v2) {
        glm::vec3 v3 = 2.0f * glm::normalize((vertices[v1] + vertices[v2]));
        vertices.push_back(v3);
        return (int)vertices.size() - 1;
    }
    std::vector<int> SubdivideIcosahedron(std::vector<int>& icosahedron) {
        std::vector<int> res;
        for (int i = 0; i < (int)icosahedron.size(); i += 3) {
            int a = FindMiddle(icosahedron[i], icosahedron[i + 1]);
            int b = FindMiddle(icosahedron[i + 1], icosahedron[i + 2]);
            int c = FindMiddle(icosahedron[i + 2], icosahedron[i]);
            res.push_back(icosahedron[i]); 
            res.push_back(a);
            res.push_back(c);
            
            res.push_back(icosahedron[i + 1]);
            res.push_back(b); 
            res.push_back(a);
            
            res.push_back(icosahedron[i + 2]); 
            res.push_back(c);
            res.push_back(b);

            res.push_back(a);
            res.push_back(b); 
            res.push_back(c);
        }
        return res;
    }
    std::vector<int> ConstructInitialIcosahedron() {
        float t = (1.0f + glm::sqrt(5.0f)) / 2.0f;
        vertices = {
            {-1, t, 0}, {1, t, 0}, {-1, -t, 0}, {1, -t, 0}, 
            {0, -1, t}, {0, 1, t}, {0, -1, -t}, {0, 1, -t},
            {t, 0, -1}, {t, 0, 1}, {-t, 0, -1}, {-t, 0, 1},
        };
        std::vector<int> res = {
            0, 11, 5,   0, 5, 1,    0, 1, 7,    0, 7, 10,   0, 10, 11,
            1, 5, 9,    5, 11, 4,   11, 10, 2,   10, 7, 6,   7, 1, 8,
            3, 9, 4,    3, 4, 2,    3, 2, 6,     3, 6, 8,    3, 8, 9,
            4, 9, 5,    2, 4, 11,   6, 2, 10,   8, 6, 7,    9, 8, 1
        };
        return res;
    }
public:
    ColorfulIcosphere(glm::vec3 _position, float _radius, glm::vec3 _color, glm::vec3 _rotation = glm::vec3(0.0f), int nb_subdivisions = 4) 
        : Object_3D(_position, _rotation), radius(_radius), color(_color) {
        this->vao = new VertexArray();

        faces = ConstructInitialIcosahedron();
        for (int i = 0; i < nb_subdivisions; ++i) faces = SubdivideIcosahedron(faces);

        int vertices_size = 3 * (int)vertices.size();
        int indexes_size = (int)faces.size();

        float* vertices_arr = new float[3 * faces.size()];
        int vertices_offset = 0;
        for (int i : faces) {
            vertices_arr[vertices_offset++] = vertices[i].x;
            vertices_arr[vertices_offset++] = vertices[i].y;
            vertices_arr[vertices_offset++] = vertices[i].z;
        }

        vertices_vbo = new VertexBuffer(&vertices_arr[0], vertices_offset * sizeof(float));
        BufferLayout vertices_layout;
        vertices_layout.AddElement<float>(3, vertices_location);
        vao->AddBuffer(*vertices_vbo, vertices_layout);

        // this->ibo = new IndexBuffer(&(unsigned int)faces[0], indexes_size);
        for (int i = 0; i < (int)faces.size(); ++i) colors_buffer.push_back(i);
        colors_ubo = new UniformBuffer((int)colors_buffer.size() * sizeof(int));
        colors_ubo->ModifyData(0, (int)colors_buffer.size() * sizeof(int), &colors_buffer[0]);

    }

    ~ColorfulIcosphere() {
        delete vao;
        // delete ibo;
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
        colors_ubo->Bind(prg, "u_colors", 2);
        prg.FillUniformMat4f("u_model", model_m);
    }

    void Render() override {
        vao->Bind();
        // ibo->Bind();
        // glCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
        glCall(glDrawArrays(GL_TRIANGLES, 0, (int)faces.size()));
        // glCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
        vao->Unbind(); 
    }
};

}
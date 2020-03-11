#pragma once
#include "Core/Common.h"
#include "Core/ShaderProgram.h"
#include "Utilities/Maths.h"

namespace N3D {

class Plane : public Object_3D {
protected:
    VertexArray* vao = nullptr;
    VertexBuffer* vertices_vbo = nullptr;

    std::vector<glm::vec3> vertices;
    glm::vec3 color;
public:
    Plane(std::vector<glm::vec3> _vertices, glm::vec3 _color) 
        : Object_3D(glm::vec3(0.0f), glm::vec3(0.0f)), vertices(_vertices), color(_color) {
        ASSERT(vertices.size() == 4);
        this->vao = new VertexArray();

        float* vertices_arr = new float[12];
        for (int i = 0; i < 3 * vertices.size(); ++i) {
            vertices_arr[i] = vertices[i / 3][i % 3];
        }

        vertices_vbo = new VertexBuffer(vertices_arr, 12 * sizeof(float));
        BufferLayout vertices_layout;
        vertices_layout.AddElement<float>(3, vertices_location);
        vao->AddBuffer(*vertices_vbo, vertices_layout);
        delete [] vertices_arr;
    }

    ~Plane() {
        delete vertices_vbo;
        delete vao;
    }

    void FillShader(ShaderProgram& prg) {
        glm::mat4 model_m = this->GetModelMatrix();
        prg.FillUniformMat4f("u_model", model_m);
        prg.FillUniformVec3("u_color", this->color);
        prg.FillUniform1i("u_texture_is_used", 0);
    }

    void Render() override {
        vao->Bind();
        // ibo->Bind();
        // glCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
        glCall(glDrawArrays(GL_TRIANGLE_FAN, 0, 4));
        // glCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
        vao->Unbind();
    }
};

}
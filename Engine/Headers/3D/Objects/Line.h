#pragma once
#include "Core/Common.h"
#include "Core/ShaderProgram.h"
#include "Utilities/Maths.h"

namespace N3D {

class Line : public Object_3D {
protected:
    VertexArray* vao = nullptr;
    VertexBuffer* vertices_vbo = nullptr;

    glm::vec3 p0, p1;
    float width;
    glm::vec3 color;
public:
    Line(glm::vec3 _p0, glm::vec3 _p1, float _width, glm::vec3 _color)
        : Object_3D(glm::vec3(0.0f), glm::vec3(0.0f)), p0(_p0), p1(_p1), width(_width), color(_color) {
        this->vao = new VertexArray();

        float vertices_arr[12] = {
            p0.x - width, p0.y - width, p0.z - width,
            p0.x + width, p0.y + width, p0.z + width,
            p1.x - width, p1.y - width, p1.z - width,
            p1.x + width, p1.y + width, p1.z + width,
        };

        vertices_vbo = new VertexBuffer(vertices_arr, 12 * sizeof(float));
        BufferLayout vertices_layout;
        vertices_layout.AddElement<float>(3, vertices_location);
        vao->AddBuffer(*vertices_vbo, vertices_layout);
    }

    ~Line() {
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
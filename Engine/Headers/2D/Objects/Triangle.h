#pragma once
#include "Core/Common.h"
#include "Core/ShaderProgram.h"
#include "Utilities/Maths.h"

namespace N3D {

class Triangle : public Object_2D {
protected:
    VertexArray* vao;
    VertexBuffer* vertices_vbo;
    IndexBuffer* ibo;

    glm::vec3 color;
    std::vector<glm::vec2> vertices;
public: 
    Triangle(glm::vec2 v0, glm::vec2 v1, glm::vec2 v2, glm::vec3 _color) 
        : Object_2D(glm::vec2(0.0f)), vertices{ v0, v1, v2 }, color(_color) {
        // position = glm::vec2(0.0f);//(v0 + v1 + v2) / 3.0f;

        this->vao = new VertexArray();

        int vertices_length = 9;
        int indexes_length = 3;
        float vertices[] = {
            v0.x, v0.y, 0,
            v1.x, v1.y, 0,
            v2.x, v2.y, 0,
        };
        unsigned int indexes[] = {
            0, 1, 2
        };

        vertices_vbo = new VertexBuffer(vertices, vertices_length * sizeof(float));
        BufferLayout vertices_layout;
        vertices_layout.AddElement<float>(3, vertices_location);
        vao->AddBuffer(*vertices_vbo, vertices_layout);

        this->ibo = new IndexBuffer(indexes, indexes_length);
    }

    ~Triangle() {
        delete vao;
        delete ibo;
        delete vertices_vbo;
    }

    glm::mat4 GetModelMatrix() override {
        return glm::translate(glm::identity<glm::mat4>(), glm::vec3(position.x, position.y, 0));
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
        //glCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
        glCall(glDrawElements(GL_TRIANGLES, ibo->GetCount(), GL_UNSIGNED_INT, 0));
        //glCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
        vao->Unbind();
    }

    float epsilon_val = 1.0f;

    bool InBounds(float x, float y) override {
        glm::vec2 v0 = vertices[0];
        glm::vec2 v1 = vertices[1];
        glm::vec2 v2 = vertices[2];
        glm::vec2 v = glm::vec2(x, y) - position;
        float area = TriangleArea(v0, v1, v2);
        if (area < 0) return false;
        float a1 = TriangleArea(v0, v1, v);
        float a2 = TriangleArea(v1, v2, v);
        float a3 = TriangleArea(v0, v2, v);
        return abs((a1 + a2 + a3) - area) <= epsilon_val;
    }
};

}
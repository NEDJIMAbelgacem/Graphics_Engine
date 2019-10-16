#pragma once
#include "Core/Common.h"
#include "Core/ShaderProgram.h"
#include "Utilities/Maths.h"

namespace N3D {

class Line : public Object_2D {
protected:
    VertexArray* vao;
    VertexBuffer* vertices_vbo;
    IndexBuffer* ibo;

    glm::vec2 start, end;
    float thickness;
    glm::vec3 color;
    float length;
    float rotation_angle;
public: 
    Line(glm::vec2 _start, glm::vec2 _end, float _thickness, glm::vec3 _color) 
        : Object_2D(glm::vec3(0.0f)), start(_start), end(_end), thickness(_thickness), color(_color) {
        glm::vec2 v = end - start;
        length = glm::length(v);
        v /= length;
        float theta_cos = glm::acos(v.x);
        float theta_sin = glm::asin(v.y);
        float theta = theta_cos;
        if (theta_sin < 0) theta *= -1.0f;
        rotation_angle = theta;

        this->vao = new VertexArray();
        int vertices_length = 12;
        int indexes_length = 6;
        float vertices[] = {
            0.0f, -0.5f, 0.0f,
            1.0f, -0.5f, 0.0f,
            1.0f,  0.5f, 0.0f,
            0.0f,  0.5f, 0.0
        };
        unsigned int indexes[] = {
            0, 1, 2, 0, 2, 3
        };

        vertices_vbo = new VertexBuffer(vertices, vertices_length * sizeof(float));
        BufferLayout vertices_layout;
        vertices_layout.AddElement<float>(3, vertices_location);
        vao->AddBuffer(*vertices_vbo, vertices_layout);

        this->ibo = new IndexBuffer(indexes, indexes_length);
    }

    ~Line() {
        delete vao;
        delete ibo;
        delete vertices_vbo;
    }

    glm::mat4 GetModelMatrix() {
        glm::mat4 scale = glm::scale(glm::identity<glm::mat4>(), glm::vec3(length, thickness, 1));
        glm::mat4 rotation = glm::rotate(glm::identity<glm::mat4>(), rotation_angle, glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 translation = glm::translate(glm::identity<glm::mat4>(), glm::vec3(start.x + position.x, start.y + position.y, 0));
        return translation * rotation * scale;
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
    
    bool InBounds(float x, float y) override {
        glm::mat4 inv_model = this->GetModelMatrixInverse();
        glm::vec4 v = inv_model * glm::vec4(x, y, 0.0f, 1.0f);
        // TODO : handle edges properly (a circle is just a bunch of triangles)
        // x = (x - position.x) / radius;
        // y = (y - position.y) / radius;
        return !(v.x < 0 || v.y < -0.5 || v.x > 1 || v.y > 0.5);
    }
};

}
#pragma once
#include "Core/Common.h"
#include "Core/ShaderProgram.h"
#include "Utilities/Maths.h"

namespace N3D {

class Rectangle : public Object_2D {
protected:
    VertexArray* vao;
    VertexBuffer* vertices_vbo;
    IndexBuffer* ibo;

    float width;
    float height;
    glm::vec3 color;
public: 
    // color default to black
    Rectangle(glm::vec2 _position, float _width, float _height, glm::vec3 _color = glm::vec3(1.0f)) 
        : Object_2D(_position), width(_width), height(_height), color(_color) {

        this->vao = new VertexArray();

        int vertices_length = 12;
        int indexes_length = 6;
        float vertices[] = {
            0.0, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0
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

    ~Rectangle() {
        delete vao;
        delete ibo;
        delete vertices_vbo;
    }

    glm::mat4 GetModelMatrix() {
        glm::mat4 scale = glm::scale(glm::identity<glm::mat4>(), glm::vec3(width, height, 1));
        glm::mat4 translation = glm::translate(glm::identity<glm::mat4>(), glm::vec3(position.x, position.y, 0));
        return translation * scale;
    }

    void SetWidth(float _width) { this->width = _width; }
    void SetHeight(float _height) { this->height = _height; }

    void FillShader(ShaderProgram& prg) override {
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
        float _x = x - position.x;
        float _y = y - position.y;
        if (_x < 0 || _x > width) return false;
        if (_y < 0 || _y > height) return false;
        return true;
    }
};

}
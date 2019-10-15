#pragma once
#include "Core/Common.h"
#include "Core/ShaderProgram.h"
#include "Utilities/Maths.h"

namespace N3D {

class Circle : public Object_2D {
protected:
    VertexArray* vao;
    VertexBuffer* vertices_vbo;
    IndexBuffer* ibo;

    float radius;
    glm::vec3 color;
public: 
    Circle(glm::vec2 _position, float _radius, glm::vec3 _color, int nb_triangles = 50) 
        : Object_2D(_position), radius(_radius), color(_color) {
        this->vao = new VertexArray();

        int vertices_size = 3* (nb_triangles + 1);
        int indexes_size = 3 * nb_triangles;
        float* vertices = new float[vertices_size];
        unsigned int* indexes = new unsigned int[indexes_size];

        for (int i = 0; i < nb_triangles; ++i) {
            float theta = i * 2.0f * glm::pi<float>() / nb_triangles;
            vertices[3 * i] = glm::cos(theta);
            vertices[3 * i + 1] = glm::sin(theta);
            vertices[3 * i + 2] = 0.0f; // TODO : handle visibility using z
            indexes[3 * i] = nb_triangles;
            indexes[3 * i + 1] = i;
            indexes[3 * i + 2] = (i + 1) % nb_triangles;
        }
        // center point
        vertices[3 * nb_triangles] = 0.0f;
        vertices[3 * nb_triangles + 1] = 0.0f;
        vertices[3 * nb_triangles + 2] = 0.0f;

        vertices_vbo = new VertexBuffer(vertices, vertices_size * sizeof(float));
        BufferLayout vertices_layout;
        vertices_layout.AddElement<float>(3, vertices_location);
        vao->AddBuffer(*vertices_vbo, vertices_layout);

        this->ibo = new IndexBuffer(indexes, indexes_size);
    }

    ~Circle() {
        delete vao;
        delete ibo;
        delete vertices_vbo;
    }

    glm::mat4 GetModelMatrix() {
        glm::mat4 scale = glm::scale(glm::identity<glm::mat4>(), glm::vec3(radius, radius, 1));
        glm::mat4 translation = glm::translate(glm::identity<glm::mat4>(), glm::vec3(position.x, position.y, 0));
        return translation * scale;
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
        // TODO : handle edges properly (a circle is just a bunch of triangles)
        x = (x - position.x) / radius;
        y = (y - position.y) / radius;
        return x * x + y * y < 1;
    }
};

}
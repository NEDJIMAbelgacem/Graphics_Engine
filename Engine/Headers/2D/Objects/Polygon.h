#pragma once
#include "Core/Common.h"
#include "Core/ShaderProgram.h"
#include "Utilities/Maths.h"

namespace N3D {


// TODO : handle concave shapes
class Polygon : public Object_2D {
protected:
    VertexArray* vao;
    VertexBuffer* vertices_vbo;
    IndexBuffer* ibo;

    std::vector<glm::vec2> points;
    glm::vec3 color;
public: 
    Polygon(std::vector<glm::vec2> _points, glm::vec3 _color) 
        : Object_2D(glm::vec3(0.0f)), points(_points), color(_color) {

        this->vao = new VertexArray();
        int vertices_length = 3 * (int)points.size();
        int indexes_length = 3 * ((int)points.size() - 2);
        float* vertices = new float[vertices_length];
        unsigned int* indexes = new unsigned int[indexes_length];
        for (int i = 0; i < points.size(); ++i) {
            vertices[3 * i] = points[i].x;
            vertices[3 * i + 1] = points[i].y;
            vertices[3 * i + 2] = 0.0f;
        }
        for (int i = 0; i < points.size() - 2; ++i) {
            indexes[3 * i] = 0;
            indexes[3 * i + 1] = i + 1;
            indexes[3 * i + 2] = i + 2;
        }

        vertices_vbo = new VertexBuffer(vertices, vertices_length * sizeof(float));
        BufferLayout vertices_layout;
        vertices_layout.AddElement<float>(3, vertices_location);
        vao->AddBuffer(*vertices_vbo, vertices_layout);

        this->ibo = new IndexBuffer(indexes, indexes_length);
        delete vertices;
        delete indexes;
    }

    ~Polygon() {
        delete vao;
        delete ibo;
        delete vertices_vbo;
    }

    glm::mat4 GetModelMatrix() {
        glm::mat4 translation = glm::translate(glm::identity<glm::mat4>(), glm::vec3(position.x, position.y, 0));
        return translation;
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
        glCall(glDrawElements(GL_TRIANGLE_FAN, ibo->GetCount(), GL_UNSIGNED_INT, 0));
        //glCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
        vao->Unbind();
    }
    
    bool InBounds(float x, float y) override {
        glm::mat4 inv_model = this->GetModelMatrixInverse();
        glm::vec4 v4 = inv_model * glm::vec4(x, y, 0.0f, 1.0f);
        
        // calculate area of triangles containing v and triangles of the polygon
        // inefficient implementaion
        // TODO : get a better algorithm
        float areas_sum = 0.0f;
        glm::vec2 v0 = {v4.x, v4.y};
        for (int i = 0; i < points.size(); ++i) {
            glm::vec2 v1 = points[i];
            glm::vec2 v2 = points[(i + 1) % points.size()];
            areas_sum += TriangleArea(v0, v1, v2);
        }
        float polygon_area = 0.0f;
        for (int i = 2; i < points.size(); ++i) {
            glm::vec2 v1 = points[0];
            glm::vec2 v2 = points[i - 1];
            glm::vec2 v3 = points[i];
            polygon_area += TriangleArea(v1, v2, v3);
        }

        return abs(polygon_area - areas_sum) <= 1.0f;
    }
};

}
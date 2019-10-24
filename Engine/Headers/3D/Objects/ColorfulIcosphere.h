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
    UniformBuffer* selected_triangles_ubo = nullptr;

    std::vector<int> colors_buffer;
    std::map<int, int> parent_triangle;
    std::map<int, std::vector<int>> children_triangles;

    std::vector<int> selected_triangles;
    
    float radius;
    glm::vec3 color;
protected:
    std::vector<glm::vec3> vertices;
    // TODO : add normals
    std::vector<glm::vec3> normals;
    std::vector<int> faces;
    std::vector<int> subdivisions_offsets;
    int last_subdivision_offset = 0;
protected:
    int FindMiddle(int v1, int v2) {
        glm::vec3 v3 = 2.0f * glm::normalize((vertices[v1] + vertices[v2]));
        vertices.push_back(v3);
        return (int)vertices.size() - 1;
    }
    // returns the offset of the last processed subdivision
    int SubdivideIcosahedron(std::vector<int>& icosahedron, int offset) {
        int nb_elements = (int)icosahedron.size() - offset;
        int res = (int)icosahedron.size();
        for (int i = offset; i < offset + nb_elements; i += 3) {
            int a = FindMiddle(icosahedron[i], icosahedron[i + 1]);
            int b = FindMiddle(icosahedron[i + 1], icosahedron[i + 2]);
            int c = FindMiddle(icosahedron[i + 2], icosahedron[i]);

            int current_triangle = i / 3;
            

            children_triangles[current_triangle].push_back((int)icosahedron.size() / 3);
            parent_triangle[(int)icosahedron.size() / 3] = current_triangle;
            icosahedron.push_back(icosahedron[i]);
            icosahedron.push_back(a);
            icosahedron.push_back(c);
            
            children_triangles[current_triangle].push_back((int)icosahedron.size() / 3);
            parent_triangle[(int)icosahedron.size() / 3] = current_triangle;
            icosahedron.push_back(icosahedron[i + 1]);
            icosahedron.push_back(b); 
            icosahedron.push_back(a);
            
            children_triangles[current_triangle].push_back((int)icosahedron.size() / 3);
            parent_triangle[(int)icosahedron.size() / 3] = current_triangle;
            icosahedron.push_back(icosahedron[i + 2]); 
            icosahedron.push_back(c);
            icosahedron.push_back(b);

            children_triangles[current_triangle].push_back((int)icosahedron.size() / 3);
            parent_triangle[(int)icosahedron.size() / 3] = current_triangle;
            icosahedron.push_back(a);
            icosahedron.push_back(b); 
            icosahedron.push_back(c);
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
            4, 9, 5,    2, 4, 11,   6, 2, 10,  9, 8, 1,     7, 6, 8
        };
        return res;
    }
public:
    ColorfulIcosphere(glm::vec3 _position, float _radius, glm::vec3 _color, glm::vec3 _rotation = glm::vec3(0.0f), int nb_subdivisions = 2) 
        : Object_3D(_position, _rotation), radius(_radius), color(_color) {
        this->vao = new VertexArray();

        faces = ConstructInitialIcosahedron();
        for (int i = 0; i < nb_subdivisions; ++i) {
            subdivisions_offsets.push_back(last_subdivision_offset);
            last_subdivision_offset = SubdivideIcosahedron(faces, last_subdivision_offset);
        }

        // for (auto [tri, par] : parent_triangle) {
        //     std::cerr << tri << " -> " << par << " -> ";
        //     for (int i : children_triangles[par]) {
        //         std::cerr << i << " ";
        //     }
        //     std::cerr << std::endl;
        // }


        int vertices_size = 3 * (int)vertices.size();

        float* vertices_arr = new float[3 * faces.size()];
        int vertices_offset = 0;
        for (int i = last_subdivision_offset; i < faces.size(); ++i) {
            vertices_arr[vertices_offset++] = vertices[faces[i]].x;
            vertices_arr[vertices_offset++] = vertices[faces[i]].y;
            vertices_arr[vertices_offset++] = vertices[faces[i]].z;
        }

        vertices_vbo = new VertexBuffer(&vertices_arr[0], vertices_offset * sizeof(float));
        BufferLayout vertices_layout;
        vertices_layout.AddElement<float>(3, vertices_location);
        vao->AddBuffer(*vertices_vbo, vertices_layout);

        for (int i = last_subdivision_offset / 3; i < (int)faces.size() / 3; ++i) {
            colors_buffer.push_back(i);
        }
        colors_ubo = new UniformBuffer((int)colors_buffer.size() * sizeof(int), 2);

        selected_triangles_ubo = new UniformBuffer((int)4000 * sizeof(int), 3);
        LoadUniformBufferData();

        delete [] vertices_arr;

    }

    ~ColorfulIcosphere() {
        delete vao;
        // delete ibo;
        delete vertices_vbo;
    }

    void LoadUniformBufferData() {
        int* colors = new int[colors_buffer.size()];
        for (int i = 0; i < colors_buffer.size(); ++i) colors[i] = colors_buffer[i];
        colors_ubo->ModifyData(0, (int)colors_buffer.size() * sizeof(int), &colors[0]);
        delete [] colors;

        int* selected_triangles_arr = new int[1 + selected_triangles.size()];
        selected_triangles_arr[0] = (int)selected_triangles.size();
        for (int i = 0; i < selected_triangles.size(); ++i) selected_triangles_arr[1 + i] = selected_triangles[i];
        selected_triangles_ubo->ModifyData(0, (1 + (int)selected_triangles.size()) * sizeof(int), &selected_triangles_arr[0]);
        delete [] selected_triangles_arr;
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
        colors_ubo->Bind(prg, "u_colors");
        selected_triangles_ubo->Bind(prg, "u_selected_triangles");
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

    int selected_triangle = -1;
    int mouse_over_triangle = -1;

    bool DoIntersect(glm::vec3 origin, glm::vec3 ray) {
        origin = this->GetModelMatrixInverse() * glm::vec4(origin, 1.0f);
        ray = this->GetModelMatrixInverse() * glm::vec4(ray, 0.0f);
        ray = glm::normalize(ray);
        int intersected_triangle = -1;
        float distance_from_origin = 10e6;
        for (int i = last_subdivision_offset; i < faces.size(); i += 3) {
            glm::vec3 v0 = vertices[faces[i]];
            glm::vec3 v1 = vertices[faces[i + 1]];
            glm::vec3 v2 = vertices[faces[i + 2]];

            float t = Ray_Plane_Intersection(origin, ray, v0, v1, v2);
            if (t < 0) continue;
            float alpha, beta, gamma;
            glm::vec3 p = origin + t * ray;
            if (!GetBaryCentricCoordinates(p, v0, v1, v2, alpha, beta, gamma)) continue;
            float d = glm::distance(p, origin);
            if (d < distance_from_origin) {
                distance_from_origin = d;
                intersected_triangle = i / 3;
            }
            
        }
        // N3D_CORE_TRACE("Intersected triangle : {}", intersected_triangle);
        selected_triangle = intersected_triangle;
        return true;
    }

    bool HandleMousePressedEvent(MouseButtonPressedEvent& e) {
        if (selected_triangle != -1) ChangleTriangleColor(selected_triangle);
        return true;
    }

    bool HandleMouseMovedEvent(MouseMovedEvent& e) {
        const int parent_level = 3;
        if (selected_triangle != -1) {
            selected_triangles.clear();
            int parent = parent_triangle[selected_triangle];
            for (int child : children_triangles[parent]) {
                selected_triangles.push_back(child - last_subdivision_offset / 3);
            }
            LoadUniformBufferData();
        }
        return true;
    }

    void ChangleTriangleColor(int triangle_index) {
        colors_buffer[triangle_index - last_subdivision_offset / 3]++;
        LoadUniformBufferData();
    }

    std::vector<glm::vec3> GetTrianglesCenters() {
        std::vector<glm::vec3> res;
        glm::mat4 m = this->GetModelMatrix();
        for (int i = last_subdivision_offset; i < faces.size(); i += 3) {
            glm::vec3 v0 = vertices[faces[i]];
            // res.push_back(glm::vec3(m * glm::vec4(v0, 1.0f)));
            glm::vec3 v1 = vertices[faces[i + 1]];
            glm::vec3 v2 = vertices[faces[i + 2]];
            glm::vec4 center = m * glm::vec4((v0 + v1 + v2) / 3.0f, 1.0f);
            res.push_back(1.01f * glm::vec3(center.x, center.y, center.z));
        }
        return res;
    }

    std::vector<glm::vec3> GetTrianglesNormals() {
        std::vector<glm::vec3> res;
        for (int i = last_subdivision_offset; i < faces.size(); i += 3) {
            glm::vec3 v0 = vertices[faces[i]];
            glm::vec3 v1 = vertices[faces[i + 1]];
            glm::vec3 v2 = vertices[faces[i + 2]];
            glm::vec3 center = (1.0f / 3.0f) * (v0 + v1 + v2);
            glm::vec4 v = this->GetModelMatrix() * glm::vec4(center.x, center.y, center.z, 1.0f);
            center = glm::vec3(v.x, v.y, v.z);
            res.push_back(glm::normalize(center));
        }
        return res;
    }

    std::vector<glm::vec3> GetTrianglesTangents() {
        std::vector<glm::vec3> res;
        for (int i = last_subdivision_offset; i < faces.size(); i += 3) {
            glm::vec3 v0 = vertices[faces[i]];
            glm::vec3 v1 = vertices[faces[i + 1]];
            glm::vec3 v2 = vertices[faces[i + 2]];
            glm::vec3 center = (1.0f / 3.0f) * (v0 + v1 + v2);
            glm::vec3 tangent = center - v0;
            glm::vec4 t = this->GetModelMatrix() * glm::vec4(tangent, 0.0f);
            res.push_back(glm::normalize(glm::vec3(t.x, t.y, t.z)));
        }
        return res;
    }
};

}
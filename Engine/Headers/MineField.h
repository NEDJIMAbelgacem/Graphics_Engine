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

class MineField : public Object_3D {
protected:
    VertexArray* vao = nullptr;
    VertexBuffer* vertices_vbo = nullptr;
    VertexBuffer* normals_vbo = nullptr;
    IndexBuffer* ibo = nullptr;
    UniformBuffer* colors_ubo = nullptr;
    UniformBuffer* selected_tiles_ubo = nullptr;

    std::vector<int> colors_buffer;
    std::map<int, int> parent_tile;
    std::map<int, std::vector<int>> children_tiles;

    std::vector<int> selected_tiles;

    std::map<int, std::function<void(int, int)>> tiles_actions;
    
    float radius;
    glm::vec3 color;

    const int tile_size = 3;
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
    int SubdivideTiles(std::vector<int>& tiles, int offset) {
        int nb_elements = (int)tiles.size() - offset;
        int res = (int)tiles.size();
        for (int i = offset; i < offset + nb_elements; i += tile_size) {
            int a = FindMiddle(tiles[i], tiles[i + 1]);
            int b = FindMiddle(tiles[i + 1], tiles[i + 2]);
            int c = FindMiddle(tiles[i + 2], tiles[i]);

            int current_tile = i / tile_size;
            

            children_tiles[current_tile].push_back((int)tiles.size() / 3);
            parent_tile[(int)tiles.size() / 3] = current_tile;
            tiles.push_back(tiles[i]);
            tiles.push_back(a);
            tiles.push_back(c);
            
            children_tiles[current_tile].push_back((int)tiles.size() / 3);
            parent_tile[(int)tiles.size() / 3] = current_tile;
            tiles.push_back(tiles[i + 1]);
            tiles.push_back(b); 
            tiles.push_back(a);
            
            children_tiles[current_tile].push_back((int)tiles.size() / 3);
            parent_tile[(int)tiles.size() / 3] = current_tile;
            tiles.push_back(tiles[i + 2]); 
            tiles.push_back(c);
            tiles.push_back(b);

            children_tiles[current_tile].push_back((int)tiles.size() / 3);
            parent_tile[(int)tiles.size() / 3] = current_tile;
            tiles.push_back(a);
            tiles.push_back(b); 
            tiles.push_back(c);
        }
        return res;
    }
    std::vector<int> ConstructInitialTiles() {
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

    std::map<int, std::vector<int>> tile_neighbours;
    void ConstructTileNeighboursMap() {
        int tile_verices_index = last_subdivision_offset;
        for (int i = tile_verices_index; i < faces.size(); i += 3) {
            for (int j = i + 3; j < faces.size(); j += 3) {
                int nb_alike = 0;
                for (int a = i; a < i + 3; ++a) {
                    for (int b = j; b < j + 3; ++b) {
                        glm::vec3 v1 = vertices[faces[a]];
                        glm::vec3 v2 = vertices[faces[b]];
                        if (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z) {
                            nb_alike++;
                        }
                    }
                }
                if (nb_alike == 2) {
                    tile_neighbours[(i - last_subdivision_offset) / 3].push_back((j - last_subdivision_offset) / 3);
                    tile_neighbours[(j - last_subdivision_offset) / 3].push_back((i - last_subdivision_offset) / 3);
                }
            }
        }
    }
public:
    MineField(glm::vec3 _position, float _radius, glm::vec3 _color, glm::vec3 _rotation = glm::vec3(0.0f), int nb_subdivisions = 2) 
        : Object_3D(_position, _rotation), radius(_radius), color(_color) {
        this->vao = new VertexArray();

        faces = ConstructInitialTiles();
        for (int i = 0; i < nb_subdivisions; ++i) {
            subdivisions_offsets.push_back(last_subdivision_offset);
            last_subdivision_offset = SubdivideTiles(faces, last_subdivision_offset);
        }

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
        delete [] vertices_arr;

        ConstructTileNeighboursMap();
        InitColorsBuffer();
    }

    ~MineField() {
        delete vao;
        // delete ibo;
        delete vertices_vbo;
    }

    void InitColorsBuffer() {
        for (int i = last_subdivision_offset / 3; i < (int)faces.size() / 3; ++i) {
            colors_buffer.push_back(0);
        }
        colors_ubo = new UniformBuffer((int)colors_buffer.size() * sizeof(int), 3);

        selected_tiles_ubo = new UniformBuffer((int)4000 * sizeof(int), 2);
        LoadColorsBuffer();
        LoadSelectedTilesBuffer();
    }

    void LoadColorsBuffer() {
        int* colors = new int[colors_buffer.size()];
        for (int i = 0; i < colors_buffer.size(); ++i) colors[i] = colors_buffer[i];
        colors_ubo->ModifyData(0, (int)colors_buffer.size() * sizeof(int), &colors[0]);
        delete [] colors;
    }

    void LoadSelectedTilesBuffer() {
        int* selected_tiles_arr = new int[1 + selected_tiles.size()];
        selected_tiles_arr[0] = (int)selected_tiles.size();
        for (int i = 0; i < selected_tiles.size(); ++i) selected_tiles_arr[1 + i] = selected_tiles[i];
        selected_tiles_ubo->ModifyData(0, (1 + (int)selected_tiles.size()) * sizeof(int), &selected_tiles_arr[0]);
        delete [] selected_tiles_arr;
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
        selected_tiles_ubo->Bind(prg, "u_selected_tiles");
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

    int selected_tile = -1;
    int mouse_over_tile = -1;

    bool DoIntersect(glm::vec3 origin, glm::vec3 ray) {
        origin = this->GetModelMatrixInverse() * glm::vec4(origin, 1.0f);
        ray = this->GetModelMatrixInverse() * glm::vec4(ray, 0.0f);
        ray = glm::normalize(ray);
        int intersected_tile = -1;
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
                intersected_tile = i / 3;
            }
            
        }
        selected_tile = intersected_tile;
        return true;
    }

    bool HandleMousePressedEvent(MouseButtonPressedEvent& e) {
        if (tiles_actions.find(selected_tile - last_subdivision_offset / 3) != tiles_actions.end()) {
            tiles_actions[selected_tile - last_subdivision_offset / 3](e.GetMouseButton(), selected_tile - last_subdivision_offset / 3);
        }
        return true;
    }

    bool HandleMouseMovedEvent(MouseMovedEvent& e) {
        const int parent_level = 3;
        if (selected_tile != -1) {
            selected_tiles.clear();
            // int parent = parent_tile[selected_tile];
            // for (int child : children_tiles[parent]) {
            //     selected_tiles.push_back(child - last_subdivision_offset / 3);
            // }
            selected_tiles = tile_neighbours[selected_tile - last_subdivision_offset / 3];
            selected_tiles.push_back(selected_tile - last_subdivision_offset / 3);
            LoadSelectedTilesBuffer();
        }
        return true;
    }

    void ChangleTriangleColor(int tile_index, int color_index) {
        colors_buffer[tile_index] = color_index;
        LoadColorsBuffer();
    }

    int GetTriangleColorIndex(int tile_index) {
        return colors_buffer[tile_index - last_subdivision_offset / 3];
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

    int GetTilesCount() {
        return ((int)faces.size() - last_subdivision_offset) / 3;
    }

    void AddTileAction(int tile_id, std::function<void(int, int)> f) {
        this->tiles_actions[tile_id] = f;
    }

    std::vector<int> GetSmallestTileNeighbours(int tile) {
        std::vector<int> res;
        res = tile_neighbours[tile];
        res.push_back(tile);
        return res;
    }
};

}
#include "Geometry/Sphere.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Sphere::Sphere(ShaderProgram* shader, glm::vec3 position, float radius, std::string obj_path) {
    this->shader = shader;
    this->SetLightColor(glm::vec3(1.0f));
    this->SetLightPosition(glm::vec3(0.0f, 100.0f, 100.0f));
    this->SetSurfaceParameters(0.0f, 1.0f, 1.0f);
    this->SetModelMatrix(glm::mat4(1.0f));
    this->position = position;
    this->radius = radius;
    glm::mat4 m = glm::identity<glm::mat4>();
    m = glm::translate(m, glm::vec3(position));
    m = glm::scale(m, glm::vec3(radius));
    this->SetModelMatrix(m);

    this->vao = new VertexArray();
    Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(obj_path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);
    if (scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode == nullptr)
	{
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		__debugbreak();
	}
    int meshes_count = scene->mNumMeshes;
    int total_vertices_count = 0;
    int total_indexes_count = 0;
    for (int i = 0; i < meshes_count; ++i) {
        total_vertices_count += scene->mMeshes[i]->mNumVertices;
        total_indexes_count += 3 * scene->mMeshes[i]->mNumFaces;
    }
    float* vertices = new float[3 * total_vertices_count];
    float* normals = new float[3 * total_vertices_count];
    unsigned int* indexes = new unsigned int[total_indexes_count];

    int vertices_offset = 0;
    int indexes_offset = 0;
    glm::vec3 min_c = glm::vec3(99999.0f);
    glm::vec3 max_c = glm::vec3(-99999.0f);
    for (int i = 0; i < meshes_count; ++i) {
        aiMesh* mesh = scene->mMeshes[i];
        int vertices_count = mesh->mNumVertices;
        for (int i = 0; i < vertices_count; ++i) {
		    vertices[vertices_offset + 3 * i] = mesh->mVertices[i].x;
		    vertices[vertices_offset + 3 * i + 1] = mesh->mVertices[i].y;
            vertices[vertices_offset + 3 * i + 2] = mesh->mVertices[i].z;
            if (max_c[0] < mesh->mVertices[i].x) max_c[0] = mesh->mVertices[i].x;
            if (min_c[0] > mesh->mVertices[i].x) min_c[0] = mesh->mVertices[i].x;
            if (max_c[1] < mesh->mVertices[i].y) max_c[1] = mesh->mVertices[i].y;
            if (min_c[1] > mesh->mVertices[i].y) min_c[1] = mesh->mVertices[i].y;
            if (max_c[2] < mesh->mVertices[i].z) max_c[2] = mesh->mVertices[i].z;
            if (min_c[2] > mesh->mVertices[i].z) min_c[2] = mesh->mVertices[i].z;
        }

        for (int i = 0; i < vertices_count; ++i) {
		    normals[vertices_offset + 3 * i] = mesh->mNormals[i].x;
		    normals[vertices_offset + 3 * i + 1] = mesh->mNormals[i].y;
            normals[vertices_offset + 3 * i + 2] = mesh->mNormals[i].z;
        }

        int faces_count = mesh->mNumFaces;
        for (int i = 0; i < faces_count; ++i) {
            indexes[indexes_offset + 3 * i] = mesh->mFaces[i].mIndices[0];
            indexes[indexes_offset + 3 * i + 1] = mesh->mFaces[i].mIndices[1];
            indexes[indexes_offset + 3 * i + 2] = mesh->mFaces[i].mIndices[2];
        }

        vertices_offset += 3 * vertices_count;
        indexes_offset += 3 * faces_count;
    }
    std::cout << "before normalisation : " << std::endl;
    std::cout << "min : " << min_c.x << " " << min_c.y << " " << min_c.z << std::endl;
    std::cout << "min : " << max_c.x << " " << max_c.y << " " << max_c.z << std::endl;

    // normalize vertice coordinates
    // normalized vertice coordinate are in [-1.0f, 1.0f] space and centered around vec3(0.0f)
    glm::vec3 min_c2 = glm::vec3(99999.0f);
    glm::vec3 max_c2 = glm::vec3(-99999.0f);
    glm::vec3 center_offset = (max_c + min_c) / 2.0f;
    glm::vec3 size_v = max_c - min_c;
    for (int i = 0; i < total_vertices_count; ++i) {
        vertices[3 * i] = (vertices[3 * i] - center_offset[0]) / size_v[0] * 2.0f;
        vertices[3 * i + 1] = (vertices[3 * i + 1] - center_offset[1]) / size_v[1] * 2.0f;
        vertices[3 * i + 2] = (vertices[3 * i + 2] - center_offset[2]) / size_v[2] * 2.0f;
        for (int j = 0; j < 3; ++j) {
            if (vertices[3 * i + j] < min_c2[j]) min_c2[j] = vertices[3 * i + j];
            if (vertices[3 * i + j] > max_c2[j]) max_c2[j] = vertices[3 * i + j];
        }
    }
    std::cout << "before normalisation : " << std::endl;
    std::cout << "min : " << min_c2.x << " " << min_c2.y << " " << min_c2.z << std::endl;
    std::cout << "min : " << max_c2.x << " " << max_c2.y << " " << max_c2.z << std::endl;

    vertices_vbo = new VertexBuffer(vertices, vertices_offset * sizeof(float));
    BufferLayout vertices_layout;
    vertices_layout.AddElement<float>(3, vertices_location);
    vao->AddBuffer(*vertices_vbo, vertices_layout);
    delete vertices;

    // normals
    normals_vbo = new VertexBuffer(normals, vertices_offset * sizeof(float));
    BufferLayout normals_layout;
    normals_layout.AddElement<float>(3, normals_location);
    vao->AddBuffer(*normals_vbo, normals_layout);
    delete normals;

    this->ibo = new IndexBuffer(indexes, indexes_offset);
    delete indexes;
}

Sphere::~Sphere() {
    delete ibo;
    delete vertices_vbo;
    delete normals_vbo;
    delete vao;
}

void Sphere::Render() {
	shader->Bind();
    vao->Bind();
    ibo->Bind();
    //glCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
	glCall(glDrawElements(GL_TRIANGLES, ibo->GetCount(), GL_UNSIGNED_INT, 0));
    //glCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
    vao->Unbind();
	shader->Unbind();
}

void Sphere::SetPosition(glm::vec3 position) {
    this->position = position;
    glm::mat4 identity = glm::identity<glm::mat4>();
    glm::mat4 trans_m = glm::translate(identity, position);
    glm::mat4 scale_m = glm::scale(identity, glm::vec3(radius));
    this->SetModelMatrix(trans_m * scale_m);
}

void Sphere::SetRadius(float radius) {
    this->radius = radius;
    glm::mat4 identity = glm::identity<glm::mat4>();
    glm::mat4 trans_m = glm::translate(identity, position);
    glm::mat4 scale_m = glm::scale(identity, glm::vec3(radius));
    this->SetModelMatrix(trans_m * scale_m);
}

bool Sphere::ray_intersection(glm::vec3 origin, glm::vec3 ray, float& depth) {
    origin = origin - position;
    float xo = origin.x, yo = origin.y, zo = origin.z;
    float xr = ray.x, yr = ray.y, zr = ray.z;
    float a = xr * xr + yr * yr + zr * zr;
    float b = 2.0f * (xo * xr + yo * yr + zo * zr);
    float c = xo * xo + yo * yo + zo * zo - radius * radius;
    // solve equation a * d^2 + b * d + c = 0
    float delta = b * b - 4 * a * c;
    if (delta < 0) return false;
    float sqrt_delta = glm::sqrt(delta);
    float d1 = -(b + sqrt_delta) / a / 2.0f;
    float d2 = -(b - sqrt_delta) / a / 2.0f;
    float max = d1 > d2 ? d1 : d2;
    float min = d1 < d2 ? d1 : d2;
    if (max < 0 || min < 0) return false;
    depth = min;
    //intersection_point += position;
    return true;
}

float Sphere::is_point_inside(glm::vec3 point) {
    return glm::length(point - position) <= radius;
}

void Sphere::SetModelMatrix(glm::mat4 matrix) {
    this->model_matrix = matrix;
    this->shader->FillUniformMat4f("u_model", matrix);
}
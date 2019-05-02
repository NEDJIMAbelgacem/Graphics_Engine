#include "Geometry/Cube.h"

Cube::Cube(ShaderProgram* shader, glm::vec3 position, glm::vec3 size, std::string obj_path) {
    this->shader = shader;
    this->SetLightColor(glm::vec3(1.0f));
    this->SetLightPosition(glm::vec3(0.0f, 100.0f, 100.0f));
    this->SetSurfaceParameters(0.0f, 1.0f, 1.0f);
    this->position = position;
    this->size = size;
    glm::mat4 m = glm::identity<glm::mat4>();
    glm::mat4 scale_m = glm::scale(m, glm::vec3(size));
    glm::mat4 trans_m = glm::translate(m, glm::vec3(position));
    this->SetModelMatrix(trans_m * scale_m);

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

    // normalize vertice coordinates
    glm::vec3 center_offset = (max_c + min_c) / 2.0f;
    glm::vec3 size_v = max_c - min_c;
    for (int i = 0; i < total_vertices_count; ++i) {
        vertices[3 * i] = (vertices[3 * i] - center_offset[0]) / size_v[0];
        vertices[3 * i + 1] = (vertices[3 * i + 1] - center_offset[1]) / size_v[1];
        vertices[3 * i + 2] = (vertices[3 * i + 2] - center_offset[2]) / size_v[2];
    }
    
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

Cube::~Cube() {
    delete ibo;
    delete vertices_vbo;
    delete normals_vbo;
    delete vao;
}

void Cube::Render() {
	shader->Bind();
    vao->Bind();
    ibo->Bind();
    //glCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
	glCall(glDrawElements(GL_TRIANGLES, ibo->GetCount(), GL_UNSIGNED_INT, 0));
    //glCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
    vao->Unbind();
	shader->Unbind();
}

void Cube::SetPosition(glm::vec3 position) {
    this->position = position;
    glm::mat4 identity = glm::identity<glm::mat4>();
    glm::mat4 trans_m = glm::translate(identity, position);
    glm::mat4 scale_m = glm::scale(identity, size);
    this->SetModelMatrix(trans_m * scale_m);
}

void Cube::SetSize(glm::vec3 size) {
    this->size = size;
    glm::mat4 identity = glm::identity<glm::mat4>();
    glm::mat4 trans_m = glm::translate(identity, position);
    glm::mat4 scale_m = glm::scale(identity, size);
    this->SetModelMatrix(trans_m * scale_m);
}

void Cube::SetModelMatrix(glm::mat4 matrix) {
    this->model_matrix = matrix;
    this->shader->FillUniformMat4f("u_model", matrix);
}
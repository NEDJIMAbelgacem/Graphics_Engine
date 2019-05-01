#include "ModelComponent.h"

ModelComponent::ModelComponent(std::string directory, const aiMesh* mesh, const aiScene* scene) {
	vertices_count = mesh->mNumVertices;
	uv_components_count = mesh->GetNumUVChannels();
    normals_count = mesh->HasNormals() ? mesh->mNumVertices : 0;
    tangent_count = mesh->HasTangentsAndBitangents() ? mesh->mNumVertices : 0;
    indexes_count = 3 * mesh->mNumFaces;
    std::cout << "vertices_count " << vertices_count << std::endl;
    std::cout << "uv_components_count " << uv_components_count << std::endl;
    std::cout << "normals_count " << normals_count << std::endl;
    std::cout << "tangent_count " << tangent_count << std::endl;
    std::cout << "indexes_count " << indexes_count << std::endl;

    if (vertices_count == 0) {
        std::cout << "[Warning] model component has no vertices" << std::endl;
        return;
    }

    this->vao = new VertexArray();
    //this->vao->Bind();

    float* vertices = nullptr;
	float** UVCoords = nullptr;
	float* normals = nullptr;
	float* tangent = nullptr;
	float* bitangent = nullptr;
	unsigned int* indexes = nullptr;

    // vertices
    vertices = new float[3 * vertices_count];
	for (unsigned int i = 0; i < vertices_count; ++i) {
		vertices[3 * i] = mesh->mVertices[i].x;
		vertices[3 * i + 1] = mesh->mVertices[i].y;
		vertices[3 * i + 2] = mesh->mVertices[i].z;
	}
    vertices_vbo = new VertexBuffer(vertices, 3 * vertices_count * sizeof(float));
    BufferLayout vertices_layout;
    vertices_layout.AddElement<float>(3, vertices_location);
    vao->AddBuffer(*vertices_vbo, vertices_layout);
    delete vertices;

    // uv coordinates (texture coordinates)
    if (uv_components_count > 0) {
		UVCoords = new float*[uv_components_count];
		UVCoords_vbo = new VertexBuffer*[uv_components_count];
		UVCoords_count = new unsigned int[uv_components_count];
	}
	for (unsigned int i = 0; i < uv_components_count; ++i) {
		UVCoords_count[i] = mesh->mNumUVComponents[i];
		UVCoords[i] = new float[UVCoords_count[i] * mesh->mNumVertices];
		for (unsigned int j = 0; j < vertices_count; ++j) {
			for (unsigned int k = 0; k < UVCoords_count[i]; ++k) {
				UVCoords[i][UVCoords_count[i] * j + k] = mesh->mTextureCoords[i][j][k];
			}
		}
		UVCoords_vbo[i] = new VertexBuffer(UVCoords[i], UVCoords_count[i] * mesh->mNumVertices * sizeof(float));
        BufferLayout uv_coords_layout;
        uv_coords_layout.AddElement<float>(2, uv_coords_location);
        vao->AddBuffer(*UVCoords_vbo[i], uv_coords_layout);
        delete UVCoords[i];
	}
    if (uv_components_count > 0) delete UVCoords;

    // normals
	if (normals_count > 0)
		normals = new float[3 * normals_count];
	for (unsigned int i = 0; i < normals_count; ++i) {
		normals[3 * i] = mesh->mNormals[i].x;
		normals[3 * i + 1] = mesh->mNormals[i].y;
		normals[3 * i + 2] = mesh->mNormals[i].z;
	}
    if (normals_count > 0) {
        normals_vbo = new VertexBuffer(normals, 3 * normals_count * sizeof(float));
        BufferLayout normals_layout;
        normals_layout.AddElement<float>(3, normals_location);
        vao->AddBuffer(*normals_vbo, normals_layout);
        delete normals;
    }
	
    // tangents and bitangents
    if (tangent_count > 0) {
		tangent = new float[3 * tangent_count];
		bitangent = new float[3 * tangent_count];
	}
	for (unsigned int i = 0; i < tangent_count; ++i) {
		tangent[3 * i] = mesh->mTangents[i].x;
		tangent[3 * i + 1] = mesh->mTangents[i].y;
		tangent[3 * i + 2] = mesh->mTangents[i].z;
		bitangent[3 * i] = mesh->mBitangents[i].x;
		bitangent[3 * i + 1] = mesh->mBitangents[i].y;
		bitangent[3 * i + 2] = mesh->mBitangents[i].z;
	}
    if (tangent_count > 0) {
	    tangent_vbo = new VertexBuffer(tangent, 3 * tangent_count * sizeof(float));
	    bitangent_vbo = new VertexBuffer(bitangent, 3 * tangent_count * sizeof(float));

        BufferLayout tangent_layout;
        tangent_layout.AddElement<float>(3, tangent_location);
        BufferLayout bitangent_layout;
        bitangent_layout.AddElement<float>(3, bitangent_location);

        vao->AddBuffer(*tangent_vbo, tangent_layout);
        vao->AddBuffer(*bitangent_vbo, bitangent_layout);
    }

    // indexes
	if (indexes_count > 0)
		indexes = new unsigned int[indexes_count];
	for (unsigned int i = 0; i < indexes_count / 3; ++i) {
		indexes[3 * i] = mesh->mFaces[i].mIndices[0];
		indexes[3 * i + 1] = mesh->mFaces[i].mIndices[1];
		indexes[3 * i + 2] = mesh->mFaces[i].mIndices[2];
	}
    if (indexes_count > 0) {
        std::cout << "generating " << indexes_count << " indexes " << std::endl;
		this->ibo = new IndexBuffer(indexes, indexes_count);
        delete indexes;
    }
    
    // materials
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	LoadTextures(material, directory, aiTextureType_DIFFUSE);
	LoadTextures(material, directory, aiTextureType_SPECULAR);
	LoadTextures(material, directory, aiTextureType_HEIGHT);
	LoadTextures(material, directory, aiTextureType_AMBIENT);
    vao->Unbind();
}

void ModelComponent::Render(ShaderProgram* shader) {
    this->BindTextures(*shader);
	shader->Bind();
    vao->Bind();
    ibo->Bind();
	glCall(glDrawElements(GL_TRIANGLES, ibo->GetCount(), GL_UNSIGNED_INT, 0));
    vao->Unbind();
	shader->Unbind();
	this->UnbindTextures();
}

void ModelComponent::RenderBatch(Batch& batch, ShaderProgram* shader) {
    VertexBuffer& offsets_vbo = *batch.GetOffsetsBuffer();
    BufferLayout offsets_layout;
    offsets_layout.AddElement<float>(3, batch_instance_offset);
    vao->AddInstanceBuffer(offsets_vbo, offsets_layout, 1);
    int instances_count = batch.GetInstancesCount();

    shader->FillUniform1i("u_isBatchRender", 1);
    this->BindTextures(*shader);
    vao->Bind();
	shader->Bind();
    ibo->Bind();
    //offsets_vbo.Bind();
	glCall(glDrawElementsInstanced(GL_TRIANGLES, ibo->GetCount(), GL_UNSIGNED_INT, 0, instances_count));
    vao->Unbind();
	shader->Unbind();
	this->UnbindTextures();
    shader->FillUniform1i("u_isBatchRender", 0);
}

VertexBuffer* ModelComponent::GetVerticesBuffer() {
	return vertices_vbo;
}

VertexBuffer* ModelComponent::GetTexCoordsBuffer(int i) {
	return UVCoords_vbo[i];
}

VertexBuffer* ModelComponent::GetNormalsBuffer() {
	return normals_vbo;
}

IndexBuffer* ModelComponent::GetIndexesBuffer() {
	return ibo;
}

std::map<std::string, Texture*> ModelComponent::textures_map;
void ModelComponent::LoadTextures(aiMaterial* material, std::string directory, aiTextureType type) {
    for (unsigned int i = 0; i < material->GetTextureCount(type); ++i) {
		aiString str;
		material->GetTexture(type, i, &str);
		std::string texture_path = directory + std::string(str.C_Str());
		if (textures_map.find(texture_path) == textures_map.end()) {
			Texture* tex = new Texture(texture_path);
			textures_map[texture_path] = tex;
            std::cout << "loading texture " << texture_path << std::endl;
		}
		switch (type) {
		case aiTextureType_DIFFUSE:
			diffuseMaps.push_back(textures_map[texture_path]);
			break;
		case aiTextureType_SPECULAR:
			specularMaps.push_back(textures_map[texture_path]);
			break;
		case aiTextureType_HEIGHT:
			heightMaps.push_back(textures_map[texture_path]);
			break;
		case aiTextureType_AMBIENT:
			ambientMaps.push_back(textures_map[texture_path]);
			break;
		}
	}
}

unsigned int ModelComponent::GetUVcount() {
	return this->uv_components_count;
}

unsigned int ModelComponent::GetUVComponentsCount(int i) {
	return UVCoords_count[i];
}

void ModelComponent::BindTextures(ShaderProgram& shader) {
	int texture_index = 0;
	for (unsigned int i = 0; i < this->diffuseMaps.size(); ++i) {
		diffuseMaps[i]->Bind(texture_index);
		bound_textures.push_back(diffuseMaps[i]);
		shader.FillUniform1i(std::string("u_material.diffuseMaps[") + std::to_string(i) + "]", texture_index);
		texture_index++;
	}
	for (unsigned int i = 0; i < this->ambientMaps.size(); ++i) {
		ambientMaps[i]->Bind(texture_index);
		bound_textures.push_back(ambientMaps[i]);
		shader.FillUniform1i(std::string("u_material.ambientMaps[") + std::to_string(i) + "]", texture_index);
		texture_index++; 
	}
	for (unsigned int i = 0; i < this->specularMaps.size(); ++i) {
		specularMaps[i]->Bind(texture_index);
		bound_textures.push_back(specularMaps[i]);
		shader.FillUniform1i(std::string("u_material.specularMaps[") + std::to_string(i) + "]", texture_index);
		texture_index++; 
	}
	for (unsigned int i = 0; i < this->heightMaps.size(); ++i) {
		heightMaps[i]->Bind(texture_index);
		bound_textures.push_back(heightMaps[i]);
		shader.FillUniform1i(std::string("u_material.heightMaps[") + std::to_string(i) + "]", texture_index);
		texture_index++;
	}
}

void ModelComponent::UnbindTextures() {
	for (Texture* t : bound_textures) t->Unbind();
	bound_textures.clear();
}

unsigned int ModelComponent::GetIndexesCount() {
	return indexes_count;
}

unsigned int ModelComponent::GetVerticesCount() {
	return vertices_count;
}

std::vector<Texture*> ModelComponent::GetDiffuseMaps() {
	return diffuseMaps;
}

std::vector<Texture*> ModelComponent::GetSpecularMaps() {
	return specularMaps;
}

std::vector<Texture*> ModelComponent::GetHeightMaps() {
	return heightMaps;
}

std::vector<Texture*> ModelComponent::GetAmbientMaps() {
	return ambientMaps;
}

ModelComponent::~ModelComponent() {
	if (vertices_vbo != nullptr) delete vertices_vbo;
	if (normals_vbo != nullptr) delete normals_vbo;
	if (ibo != nullptr) delete ibo;
	for (unsigned int i = 0; i < uv_components_count; ++i) {
		if (UVCoords_vbo[i] != nullptr) delete UVCoords_vbo[i];
	}
	delete UVCoords_vbo;
}
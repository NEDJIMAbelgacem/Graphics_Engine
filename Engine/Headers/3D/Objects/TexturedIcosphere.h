#pragma once
#include "Core/Common.h"
#include "Core/ShaderProgram.h"
#include "Utilities/Maths.h"
#include "Sphere.h"
#include "Core/Texture.h"

namespace N3D {

#define vertices_location 0
#define uv_coords_location 1
#define normals_location 2
#define batch_instance_offset 3
#define tangent_location 4
#define bitangent_location 5

class TexturedIcosphere : public Icosphere {
protected:
    Texture* texture;
public:
    TexturedIcosphere(std::string texture_path, glm::vec3 _position, float _radius, glm::vec3 _rotation = glm::vec3(0.0f), int nb_triangles = 50) 
        : Icosphere(_position, _radius, glm::vec3(1.0f, 0.0f, 0.0f), _rotation, nb_triangles) {
        texture = new Texture(texture_path);
    }

    ~TexturedIcosphere() {

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
        prg.FillUniformMat4f("u_model", model_m);
        // prg.FillUniformVec3("u_color", this->color);
        prg.FillUniform1i("u_texture_is_used", 1);
        prg.FillUniform1i("u_texture", 3);
    }

    void Render() override {
        texture->Bind(3);
        vao->Bind();
        ibo->Bind();
        // glCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
        glCall(glDrawElements(GL_TRIANGLES, ibo->GetCount(), GL_UNSIGNED_INT, 0));
        // glCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
        vao->Unbind();
        texture->Unbind();
    }
};

}
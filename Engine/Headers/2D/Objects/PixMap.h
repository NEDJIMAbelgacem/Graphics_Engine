#pragma once
#include "Core/Common.h"
#include "Core/ShaderProgram.h"
#include "Utilities/Maths.h"
#include "Rectangle.h"
#include "Core/Texture.h"
#include "Logger.h"

namespace N3D {

class PixMap : public Rectangle {
protected:
    std::string source_file_path;
    Texture* texture = nullptr;
public: 
    PixMap(glm::vec2 position, float width, float height, std::string file_path) 
        : Rectangle(position, width, height), source_file_path(file_path) {
        texture = new Texture(file_path);
    }

    ~PixMap() {
        delete texture;
    }

    void FillShader(ShaderProgram& prg) override {
        glm::mat4 model_m = this->GetModelMatrix();
        prg.FillUniform1i("u_texture_is_used", 1);
        prg.FillUniform1i("u_texture", 2);
        prg.FillUniformMat4f("u_model", model_m);
    }

    void Render() override {
        texture->Bind(2);
        vao->Bind();
        ibo->Bind();
        //glCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
        glCall(glDrawElements(GL_TRIANGLES, ibo->GetCount(), GL_UNSIGNED_INT, 0));
        //glCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
        vao->Unbind();
        texture->Unbind();
    }
};

}
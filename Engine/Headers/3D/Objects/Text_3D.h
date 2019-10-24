#pragma once
#include "Core/Common.h"
#include "Core/ShaderProgram.h"
#include "Utilities/Maths.h"
#include "Sphere.h"
#include "Core/Texture.h"
#include "Core/VertexBuffer.h"
#include "Core/VertexArray.h"
#include "Font.h"
#include "3D/3D_Objects.h"
#include "Utilities/Maths.h"

namespace N3D {

class Text_3D : Object_3D {
protected:
    glm::vec2 bound_size;
    float scale;
    std::string text;
    glm::vec3 color;

    VertexArray* vao;
    VertexBuffer* vbo;

    Camera_3D& camera;

    glm::vec3 normal;
    glm::vec3 tangent;

    glm::vec3 pos;

    // unsigned int vao, vbo;
public:
    Text_3D(glm::vec3 _position, float _scale, std::string _text,
            Camera_3D& _camera,
            glm::vec3 _normal = glm::vec3(0.0f, 0.0f, -1.0f), 
            glm::vec3 _tangent = glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3 _color = glm::vec3(0.0f)
            ) 
        :   Object_3D(_position, glm::vec3(0.0f), glm::vec3(_scale)), 
            camera(_camera),
            scale(_scale), text(_text), color(_color),
            normal(_normal), tangent(_tangent)
    {
        vao = new VertexArray;
        vbo = new VertexBuffer(NULL, 6 * 5 * sizeof(float), VertexBufferType::DYNAMIC_DRAW);
		BufferLayout layout;
		layout.AddElement<float>(3, 0);
        layout.AddElement<float>(2, 1);
		vao->AddBuffer(*vbo, layout);
    }

    ~Text_3D() {
        // TODO : deallocate buffers
    }

    glm::mat4 GetModelMatrix() {
        glm::mat4 scale = glm::scale(glm::identity<glm::mat4>(), glm::vec3(this->scale));

        glm::vec3 a = glm::vec3(0.0f, 0.0f, 1.0f);
        glm::vec3 b = glm::normalize(position);
        glm::mat3 r = align_vectors(a, b);
        // glm::mat3 r = glm::inverse(glm::mat3(
        //     glm::normalize(this->tangent),
        //     glm::normalize(glm::cross(this->normal, this->tangent)),
        //     glm::normalize(this->normal)
        // ));
        glm::mat4 rotation = glm::mat4(
            glm::vec4(r[0][0], r[0][1], r[0][2], 0.0f),
            glm::vec4(r[1][0], r[1][1], r[1][2], 0.0f),
            glm::vec4(r[2][0], r[2][1], r[2][2], 0.0f),
            glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
        );

        // rotation = glm::transpose(rotation);

        glm::mat4 translation = glm::translate(glm::identity<glm::mat4>(), position);
        return translation * rotation * scale;
    }

    void FillShader(ShaderProgram& prg) {
        prg.FillUniformVec3("u_textColor", color);
        prg.FillUniform1i("u_text", 0);
        prg.FillUniformMat4f("u_model", GetModelMatrix());
    }

    void SetPlaneNormal(glm::vec3 normal) {
        this->normal = normal;
    }

    void Render(Font& font) {
        float x = -0.5f * font[text[0]].size.x, y = -0.5f * font[text[0]].size.y;

        // Activate corresponding render stat
        glCall(glActiveTexture(GL_TEXTURE0));
        vao->Bind();

        // Iterate through all characters
        for (char c : text) {
            Glyph& ch = font[c];

            GLfloat xpos = x + ch.bearing.x * scale;
            GLfloat ypos = y - (ch.size.y - ch.bearing.y) * scale;

            GLfloat w = ch.size.x * scale;
            GLfloat h = ch.size.y * scale;
            // Update VBO for each character
            
            GLfloat vertices[6][5] = {
                { xpos,     ypos + h,   0.0f,   0.0, 0.0 },            
                { xpos,     ypos,       0.0f,   0.0, 1.0 },
                { xpos + w, ypos,       0.0f,   1.0, 1.0 },

                { xpos,     ypos + h,   0.0f,   0.0, 0.0 },
                { xpos + w, ypos,       0.0f,   1.0, 1.0 },
                { xpos + w, ypos + h,   0.0f,   1.0, 0.0 }           
            };
            // Render glyph texture over quad
            ch.texture->Bind(0);
            vbo->ModifyData(0, 6 * 5 * sizeof(float), vertices);
            // Render quad
            glCall(glDrawArrays(GL_TRIANGLES, 0, 6));
            // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            x += (ch.advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
        }
        vao->Unbind();
        glCall(glBindTexture(GL_TEXTURE_2D, 0));
    }
};

}
#pragma once
#include "Core/Common.h"
#include "Core/ShaderProgram.h"
#include "Utilities/Maths.h"
#include "Sphere.h"
#include "Core/Texture.h"
#include "Font.h"

namespace N3D {

class Text {
protected:
    glm::vec2 position;
    glm::vec2 bound_size;
    float scale;
    std::string text;
    glm::vec3 color;

    VertexArray* vao = nullptr;
    VertexBuffer* vbo = nullptr;
public:
    Text(glm::vec2 _position, float _scale, std::string _text, glm::vec3 _color = glm::vec3(0.0f)) 
        : position(_position), scale(_scale), text(_text), color(_color) {
        vao = new VertexArray;
        vbo = new VertexBuffer(NULL, sizeof(GLfloat) * 6 * 4, VertexBufferType::DYNAMIC_DRAW);
        BufferLayout layout;
		layout.AddElement<float>(4, 0);
		vao->AddBuffer(*vbo, layout);
    }

    ~Text() {

    }

    void FillShader(ShaderProgram& prg) {
        prg.FillUniformVec3("u_textColor", color);
        prg.FillUniform1i("u_text", 0);
    }

    void Render(Font& font) {
        float x = position.x, y = position.y;

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
            GLfloat vertices[6][4] = {
                { xpos,     ypos + h,   0.0, 0.0 },            
                { xpos,     ypos,       0.0, 1.0 },
                { xpos + w, ypos,       1.0, 1.0 },

                { xpos,     ypos + h,   0.0, 0.0 },
                { xpos + w, ypos,       1.0, 1.0 },
                { xpos + w, ypos + h,   1.0, 0.0 }           
            };
            // Render glyph texture over quad
            ch.texture->Bind(0);
            // Update content of VBO memory
            vbo->ModifyData(0, 6 * 4 * sizeof(float), vertices);
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
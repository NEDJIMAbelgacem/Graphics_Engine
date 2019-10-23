#pragma once
#include "Core/Common.h"
#include "Core/ShaderProgram.h"
#include "Utilities/Maths.h"
#include "Sphere.h"
#include "Core/Texture.h"
#include "FontManager.h"

namespace N3D {

class Text {
protected:
    glm::vec2 position;
    glm::vec2 bound_size;
    float scale;
    std::string text;
    glm::vec3 color;

    unsigned int vao, vbo;
public:
    Text(glm::vec2 _position, float _scale, std::string _text, glm::vec3 _color = glm::vec3(0.0f)) 
        : position(_position), scale(_scale), text(_text), color(_color) {
        glCall(glGenVertexArrays(1, &vao));
        glCall(glGenBuffers(1, &vbo));
        glCall(glBindVertexArray(vao));
        glCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));
        glCall(glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW));
        glCall(glEnableVertexAttribArray(0));
        glCall(glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0));
        glCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
        glCall(glBindVertexArray(0));
    }

    ~Text() {

    }

    void FillShader(ShaderProgram& prg) {
        prg.FillUniformVec3("u_textColor", color);
        prg.FillUniform1i("u_text", 0);
    }

    void Render(FontManager& font) {
        float x = position.x, y = position.y;

        // Activate corresponding render stat
        glCall(glActiveTexture(GL_TEXTURE0));
        glCall(glBindVertexArray(vao));

        // Iterate through all characters
        for (char c : text)
        {
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
            // glBindTexture(GL_TEXTURE_2D, ch);
            // Update content of VBO memory
            glCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));
            glCall(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices)); 
            glCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
            // Render quad
            glCall(glDrawArrays(GL_TRIANGLES, 0, 6));
            // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            x += (ch.advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
        }
        glCall(glBindVertexArray(0));
        glCall(glBindTexture(GL_TEXTURE_2D, 0));
    }
};

}
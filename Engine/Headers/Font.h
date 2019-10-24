#pragma once
#include "Core/Common.h"
#include <ft2build.h>
#include FT_FREETYPE_H 

struct Glyph {
    Texture* texture;
    glm::ivec2 size;
    glm::ivec2 bearing;
    int advance;
};

class Font {
private:
    FT_Library ft;
    FT_Face face;
    std::map<char, Glyph> ascii_glyphs;
public:
    Font(std::string font_path = "Resources/Fonts/arial.ttf") {
        if (FT_Init_FreeType(&ft))
            N3D_CORE_FATAL("ERROR::FREETYPE: Could not init FreeType Library" );

        if (FT_New_Face(ft, font_path.c_str(), 0, &face))
            N3D_CORE_FATAL("ERROR::FREETYPE: Failed to load font");
        
        // set pixel size
        FT_Set_Pixel_Sizes(face, 0, 48);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction
        for (unsigned char c = 0; c < 128; c++) {
            // Load character glyph 
            if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
                N3D_CORE_WARN("ERROR::FREETYTPE: Failed to load Glyph {}", c);
                continue;
            }
            // Generate texture
            GLuint texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );
            // Set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // Now store character for later use
            ascii_glyphs[c] = {
                new Texture(texture, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0), 
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                face->glyph->advance.x
            };
        }

        FT_Done_Face(face);
        FT_Done_FreeType(ft);
    }

    ~Font() {

    }

    Glyph& operator[](char c) {
        return ascii_glyphs[c];
    }
};


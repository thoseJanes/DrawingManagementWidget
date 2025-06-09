#pragma once
#include "Shader.h"
#include <glm/glm.hpp> 
#include <ft2build.h>
#include FT_FREETYPE_H

struct Character {
    GLuint     TextureID;  // 字形纹理ID
    glm::ivec2 Size;       // 字形大大小
    glm::ivec2 Bearing;    // 字形基于基线和起点的位置
    GLuint     Advance;    // 起点到下一个字形起点的距离
};

class GLResource : public QOpenGLWidget
    , protected QOpenGLFunctions_4_5_Compatibility
{
public:
    Shader* axisShader;
    Shader* dataShader;
    Shader* characterShader;
    Shader* rectShader;
    Shader* simpleShader;
    std::string unicodeTff = "fonts/simkai.ttf";
    std::string asciiTff = "fonts/times.ttf";
    FT_Library ft;
    FT_Face face;
    std::map<wchar_t, Character> characters;

    explicit GLResource(QWidget* parent = nullptr) {
        QSurfaceFormat format;
        format.setSamples(4);  // 设置多重采样数量为4，常用值有2, 4, 8等
        setFormat(format);
        create();
    };
    ~GLResource() {
        FT_Done_Face(face);
        FT_Done_FreeType(ft);
        delete axisShader;
        delete dataShader;
        delete characterShader;
        delete simpleShader;
        delete rectShader;
    };
    void initializeGL() override {
        initializeOpenGLFunctions();
        axisShader = new Shader("shaders/AxisShader.vs", "shaders/AxisShader.frag", nullptr);
        dataShader = new Shader("shaders/BasicShader.vs", "shaders/BasicShader.frag", "shaders/BasicShader.geo");
        characterShader = new Shader( "shaders/CharacterShader.vs", "shaders/CharacterShader.frag", nullptr);
        simpleShader = new Shader("shaders/SimpleShader.vs", "shaders/SimpleShader.frag", nullptr);
        rectShader = new Shader("shaders/RectShader.vs", "shaders/RectShader.frag", "shaders/RectShader.geo");
        
        qDebug() << "resource init over";
        loadCharacters();
        //this->doneCurrent();
    };
    Character findCharacter(const wchar_t* text ,int i) {
        auto it = characters.find(text[i]);
        if (it == characters.end()) {
            if (text[i] > 0x7F) {
                loadUnicodeCharacters(text);
            }
            else {
                loadASCIICharacters(text);
            }
            it = characters.find(text[i]);
        }
        Character ch = it->second;
        return ch;
    }
    void loadUnicodeCharacters(const wchar_t* text) {
        int lew_w = wcslen(text);
        //qDebug() << "load chinese";
        //(c >= 0x00 && c <= 0x7F)为英文字符
        qDebug() << "load chinese";
        if (FT_Init_FreeType(&ft)) {
            qDebug() << "ERROR::FREETYPE: Could not init FreeType Library";
            std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        }

        if (FT_New_Face(ft, unicodeTff.c_str(), 0, &face)) {
            qDebug() << "ERROR::FREETYPE: Failed to load font";
            std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        }

        if (FT_Set_Pixel_Sizes(face, 0, 20)) {
            qDebug() << "ERROR::FREETYPE: Failed to set pixel sizes";
        }
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //禁用byte-alignment限制

        for (int i = 0; i < lew_w; i++)
        {
            if (text[i] <= 0x7F || characters.find(text[i]) != characters.end()) {
                //qDebug() << GLchar(text[i]) << ":" << text[i];
                continue;
            }

            FT_UInt glyph_index = FT_Get_Char_Index(face, text[i]);
            if (glyph_index == 0) {
                qDebug() << "ERROR::FREETYPE: Character not found in the font";
                continue;
            }
            // 加载字符的字形 
            if (FT_Load_Char(face, text[i], FT_LOAD_RENDER))
            {
                qDebug() << "ERROR::FREETYTPE: Failed to load Glyph";
                continue;
            }


            // 创建一个临时 RGBA 图像
            unsigned int height = face->glyph->bitmap.rows;
            unsigned int width = face->glyph->bitmap.width;
            // 生成字形纹理
            GLuint texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                width,
                height,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer//face->glyph->bitmap.buffer
            );
            // 设置纹理选项
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // 将字符存储到字符表中备用
            Character character = {
                texture,
                glm::ivec2(width, height),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                face->glyph->advance.x
            };
            characters[text[i]] = character;
        }//注意这里只生成了ASCII中的128个字符
        //清理freetype的资源
    }
    void loadASCIICharacters(const wchar_t* text) {
        int lew_w = wcslen(text);
        //qDebug() << "load chinese";
        //(c >= 0x00 && c <= 0x7F)为英文字符
        if (FT_Init_FreeType(&ft)) {
            qDebug() << "ERROR::FREETYPE: Could not init FreeType Library";
            std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        }

        if (FT_New_Face(ft, asciiTff.c_str(), 0, &face)) {
            qDebug() << "ERROR::FREETYPE: Failed to load font";
            std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        }

        if (FT_Set_Pixel_Sizes(face, 0, 20)) {
            qDebug() << "ERROR::FREETYPE: Failed to set pixel sizes";
        }
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //禁用byte-alignment限制

        for (int i = 0; i < lew_w; i++)
        {
            if (text[i] > 0x7F || characters.find(text[i]) != characters.end()) {
                //qDebug() << GLchar(text[i]) << ":" << text[i];
                continue;
            }

            FT_UInt glyph_index = FT_Get_Char_Index(face, text[i]);
            if (glyph_index == 0) {
                qDebug() << "ERROR::FREETYPE: Character not found in the font";
                continue;
            }
            // 加载字符的字形 
            if (FT_Load_Char(face, text[i], FT_LOAD_RENDER))
            {
                qDebug() << "ERROR::FREETYTPE: Failed to load Glyph";
                continue;
            }


            // 创建一个临时 RGBA 图像
            unsigned int height = face->glyph->bitmap.rows;
            unsigned int width = face->glyph->bitmap.width;
            // 生成字形纹理
            GLuint texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                width,
                height,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer//face->glyph->bitmap.buffer
            );
            // 设置纹理选项
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // 将字符存储到字符表中备用
            Character character = {
                texture,
                glm::ivec2(width, height),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                face->glyph->advance.x
            };
            characters[text[i]] = character;
        }//注意这里只生成了ASCII中的128个字符
        //清理freetype的资源
    }

    void loadCharacters() {
        if (FT_Init_FreeType(&ft)) {
            qDebug() << "ERROR::FREETYPE: Could not init FreeType Library";
            std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        }

        if (FT_New_Face(ft, asciiTff.c_str(), 0, &face)) {
            qDebug() << "ERROR::FREETYPE: Failed to load font";
            std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        }

        if (FT_Set_Pixel_Sizes(face, 0, 20)) {
            qDebug() << "ERROR::FREETYPE: Failed to set pixel sizes";
        }
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //禁用byte-alignment限制


        for (GLubyte cn = 0; cn < 128; cn++)
        {
            wchar_t c = cn;
            FT_UInt glyph_index = FT_Get_Char_Index(face, c);
            if (glyph_index == 0) {
                qDebug() << "ERROR::FREETYPE: Character not found in the font";
                continue;
            }
            // 加载字符的字形
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                qDebug() << "ERROR::FREETYTPE: Failed to load Glyph";
                continue;
            }
            //qDebug() << face->glyph->bitmap.width << ":" << face->glyph->bitmap.rows;
            //qDebug() << face->glyph->bitmap.buffer;
            //qDebug() << GLchar(c);

            // 创建一个临时 RGBA 图像
            unsigned int height = face->glyph->bitmap.rows;
            unsigned int width = face->glyph->bitmap.width;
            // 生成字形纹理
            GLuint texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                width,
                height,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer//face->glyph->bitmap.buffer
            );
            // 设置纹理选项
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // 将字符存储到字符表中备用
            Character character = {
                texture,
                glm::ivec2(width, height),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                face->glyph->advance.x
            };
            characters.insert(std::pair<wchar_t, Character>(c, character));
        }//注意这里只生成了ASCII中的128个字符
        //清理freetype的资源
    }
};
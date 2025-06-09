#pragma once
#include "Shader.h"
#include <glm/glm.hpp> 
#include <ft2build.h>
#include FT_FREETYPE_H

struct Character {
    GLuint     TextureID;  // ��������ID
    glm::ivec2 Size;       // ���δ��С
    glm::ivec2 Bearing;    // ���λ��ڻ��ߺ�����λ��
    GLuint     Advance;    // ��㵽��һ���������ľ���
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
        format.setSamples(4);  // ���ö��ز�������Ϊ4������ֵ��2, 4, 8��
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
        //(c >= 0x00 && c <= 0x7F)ΪӢ���ַ�
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
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //����byte-alignment����

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
            // �����ַ������� 
            if (FT_Load_Char(face, text[i], FT_LOAD_RENDER))
            {
                qDebug() << "ERROR::FREETYTPE: Failed to load Glyph";
                continue;
            }


            // ����һ����ʱ RGBA ͼ��
            unsigned int height = face->glyph->bitmap.rows;
            unsigned int width = face->glyph->bitmap.width;
            // ������������
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
            // ��������ѡ��
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // ���ַ��洢���ַ����б���
            Character character = {
                texture,
                glm::ivec2(width, height),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                face->glyph->advance.x
            };
            characters[text[i]] = character;
        }//ע������ֻ������ASCII�е�128���ַ�
        //����freetype����Դ
    }
    void loadASCIICharacters(const wchar_t* text) {
        int lew_w = wcslen(text);
        //qDebug() << "load chinese";
        //(c >= 0x00 && c <= 0x7F)ΪӢ���ַ�
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
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //����byte-alignment����

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
            // �����ַ������� 
            if (FT_Load_Char(face, text[i], FT_LOAD_RENDER))
            {
                qDebug() << "ERROR::FREETYTPE: Failed to load Glyph";
                continue;
            }


            // ����һ����ʱ RGBA ͼ��
            unsigned int height = face->glyph->bitmap.rows;
            unsigned int width = face->glyph->bitmap.width;
            // ������������
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
            // ��������ѡ��
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // ���ַ��洢���ַ����б���
            Character character = {
                texture,
                glm::ivec2(width, height),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                face->glyph->advance.x
            };
            characters[text[i]] = character;
        }//ע������ֻ������ASCII�е�128���ַ�
        //����freetype����Դ
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
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //����byte-alignment����


        for (GLubyte cn = 0; cn < 128; cn++)
        {
            wchar_t c = cn;
            FT_UInt glyph_index = FT_Get_Char_Index(face, c);
            if (glyph_index == 0) {
                qDebug() << "ERROR::FREETYPE: Character not found in the font";
                continue;
            }
            // �����ַ�������
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                qDebug() << "ERROR::FREETYTPE: Failed to load Glyph";
                continue;
            }
            //qDebug() << face->glyph->bitmap.width << ":" << face->glyph->bitmap.rows;
            //qDebug() << face->glyph->bitmap.buffer;
            //qDebug() << GLchar(c);

            // ����һ����ʱ RGBA ͼ��
            unsigned int height = face->glyph->bitmap.rows;
            unsigned int width = face->glyph->bitmap.width;
            // ������������
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
            // ��������ѡ��
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // ���ַ��洢���ַ����б���
            Character character = {
                texture,
                glm::ivec2(width, height),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                face->glyph->advance.x
            };
            characters.insert(std::pair<wchar_t, Character>(c, character));
        }//ע������ֻ������ASCII�е�128���ַ�
        //����freetype����Դ
    }
};
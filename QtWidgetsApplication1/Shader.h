#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <QtWidgets/QOpenGLWidget>
#include <QtGui/QOpenGLFunctions_4_5_Compatibility>

#include <QtGui/QKeyEvent>
#include <QtCore/QDebug>

class Shader 
    : protected QOpenGLFunctions_4_5_Compatibility
{
public:
    // ����ID
    GLuint Program;
    // ��������ȡ��������ɫ��
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* geometryPath) {
        //std::string vp = dirPath + "/" + vertexFile;
        //std::string fp = dirPath + "/" + fragmentFile;
        //std::string gp = dirPath + "/" + geometryFile;
        //const GLchar* vertexPath = vp.c_str();
        //const GLchar* fragmentPath = fp.c_str();
        //const GLchar* geometryPath = gp.c_str();
        //qDebug() <<"vertex:" << vertexPath;
        //QOpenGLFunctions
    //Ϊ��ǰ�����ĳ�ʼ��opengl��������
        initializeOpenGLFunctions();
        // 1. ���ļ�·���л�ȡ����/Ƭ����ɫ��
        std::string vertexCode;
        std::string fragmentCode;

        std::ifstream vShaderFile;
        std::ifstream fShaderFile;

        // ��֤ifstream��������׳��쳣��
        vShaderFile.exceptions(std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::badbit);

        try
        {
            // ���ļ�
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            // ��ȡ�ļ��Ļ������ݵ�����
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // �ر��ļ�
            vShaderFile.close();
            fShaderFile.close();
            // ת������GLchar����
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch (std::ifstream::failure e)
        {
            qDebug() << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ";
        }
        const GLchar* vShaderCode = vertexCode.c_str();
        const GLchar* fShaderCode = fragmentCode.c_str();
        // 2. ������ɫ��
        GLuint vertex, fragment;
        GLint success;
        GLchar infoLog[512];

        // ������ɫ��
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        // ��ӡ�����������еĻ���
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            qDebug() << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog;
        };

        // Ƭ����ɫ��
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        // ��ӡ�����������еĻ���
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragment, 512, NULL, infoLog);
            qDebug() << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog;
        };

        // ��ɫ������
        this->Program = glCreateProgram();
        glAttachShader(this->Program, vertex);
        glAttachShader(this->Program, fragment);
        GLuint geometry;
        if (geometryPath != nullptr) {
            std::string geometryCode;
            std::ifstream gShaderFile;
            gShaderFile.exceptions(std::ifstream::badbit);
            try {
                // ���ļ�
                gShaderFile.open(geometryPath);
                std::stringstream gShaderStream;
                // ��ȡ�ļ��Ļ������ݵ�����
                gShaderStream << gShaderFile.rdbuf();
                // �ر��ļ�
                gShaderFile.close();
                // ת������GLchar����
                geometryCode = gShaderStream.str();
            }
            catch (std::ifstream::failure e)
            {
                qDebug() << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ";
            }
            const GLchar* gShaderCode = geometryCode.c_str();
            // Ƭ����ɫ��

            geometry = glCreateShader(GL_GEOMETRY_SHADER);
            glShaderSource(geometry, 1, &gShaderCode, NULL);
            glCompileShader(geometry);
            // ��ӡ�����������еĻ���
            glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(geometry, 512, NULL, infoLog);
                qDebug() << "ERROR::SHADER::geometry::COMPILATION_FAILED\n" << infoLog;
            };

            glAttachShader(this->Program, geometry);
        }
        glLinkProgram(this->Program);
        // ��ӡ���Ӵ�������еĻ���
        glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
            qDebug() << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog;
        }

        // ɾ����ɫ���������Ѿ����ӵ����ǵĳ������ˣ��Ѿ�������Ҫ��
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        if (geometryPath != nullptr) {
            glDeleteShader(geometry);
        }
    };
    ~Shader() {
        glDeleteProgram(this->Program);
    };
    // ʹ�ó���
    void Use() {
        glUseProgram(this->Program);
    };
};

#endif
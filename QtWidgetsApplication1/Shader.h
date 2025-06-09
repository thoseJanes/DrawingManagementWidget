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
    // 程序ID
    GLuint Program;
    // 构造器读取并构建着色器
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* geometryPath) {
        //std::string vp = dirPath + "/" + vertexFile;
        //std::string fp = dirPath + "/" + fragmentFile;
        //std::string gp = dirPath + "/" + geometryFile;
        //const GLchar* vertexPath = vp.c_str();
        //const GLchar* fragmentPath = fp.c_str();
        //const GLchar* geometryPath = gp.c_str();
        //qDebug() <<"vertex:" << vertexPath;
        //QOpenGLFunctions
    //为当前上下文初始化opengl函数解析
        initializeOpenGLFunctions();
        // 1. 从文件路径中获取顶点/片段着色器
        std::string vertexCode;
        std::string fragmentCode;

        std::ifstream vShaderFile;
        std::ifstream fShaderFile;

        // 保证ifstream对象可以抛出异常：
        vShaderFile.exceptions(std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::badbit);

        try
        {
            // 打开文件
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            // 读取文件的缓冲内容到流中
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // 关闭文件
            vShaderFile.close();
            fShaderFile.close();
            // 转换流至GLchar数组
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch (std::ifstream::failure e)
        {
            qDebug() << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ";
        }
        const GLchar* vShaderCode = vertexCode.c_str();
        const GLchar* fShaderCode = fragmentCode.c_str();
        // 2. 编译着色器
        GLuint vertex, fragment;
        GLint success;
        GLchar infoLog[512];

        // 顶点着色器
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        // 打印编译错误（如果有的话）
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            qDebug() << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog;
        };

        // 片段着色器
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        // 打印编译错误（如果有的话）
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragment, 512, NULL, infoLog);
            qDebug() << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog;
        };

        // 着色器程序
        this->Program = glCreateProgram();
        glAttachShader(this->Program, vertex);
        glAttachShader(this->Program, fragment);
        GLuint geometry;
        if (geometryPath != nullptr) {
            std::string geometryCode;
            std::ifstream gShaderFile;
            gShaderFile.exceptions(std::ifstream::badbit);
            try {
                // 打开文件
                gShaderFile.open(geometryPath);
                std::stringstream gShaderStream;
                // 读取文件的缓冲内容到流中
                gShaderStream << gShaderFile.rdbuf();
                // 关闭文件
                gShaderFile.close();
                // 转换流至GLchar数组
                geometryCode = gShaderStream.str();
            }
            catch (std::ifstream::failure e)
            {
                qDebug() << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ";
            }
            const GLchar* gShaderCode = geometryCode.c_str();
            // 片段着色器

            geometry = glCreateShader(GL_GEOMETRY_SHADER);
            glShaderSource(geometry, 1, &gShaderCode, NULL);
            glCompileShader(geometry);
            // 打印编译错误（如果有的话）
            glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(geometry, 512, NULL, infoLog);
                qDebug() << "ERROR::SHADER::geometry::COMPILATION_FAILED\n" << infoLog;
            };

            glAttachShader(this->Program, geometry);
        }
        glLinkProgram(this->Program);
        // 打印连接错误（如果有的话）
        glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
            qDebug() << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog;
        }

        // 删除着色器，它们已经链接到我们的程序中了，已经不再需要了
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        if (geometryPath != nullptr) {
            glDeleteShader(geometry);
        }
    };
    ~Shader() {
        glDeleteProgram(this->Program);
    };
    // 使用程序
    void Use() {
        glUseProgram(this->Program);
    };
};

#endif
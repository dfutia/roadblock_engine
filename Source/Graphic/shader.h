#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <chrono>

class Shader {
public:
    unsigned int ID;
    std::string vertexPath;
    std::string fragmentPath;
    std::filesystem::file_time_type lastVertexModTime;
    std::filesystem::file_time_type lastFragmentModTime;

    Shader(const std::string& vertexPath, const std::string& fragmentPath) : vertexPath(vertexPath), fragmentPath(fragmentPath) {
        std::cout << "loading shader from file" << std::endl;
        std::string vertexCode = loadShader(vertexPath);
        std::string fragmentCode = loadShader(fragmentPath);
        createShaderProgram(vertexCode, fragmentCode);
        lastVertexModTime = std::filesystem::last_write_time(vertexPath);
        lastFragmentModTime = std::filesystem::last_write_time(fragmentPath);
    }

    Shader(const char* vertexCode, const char* fragmentCode, bool isSource) {
        std::cout << "loading shader from memory" << std::endl;
        createShaderProgram(vertexCode, fragmentCode);
    }

    void use() {
        auto currentVertexModTime = std::filesystem::last_write_time(vertexPath);
        auto currentFragmentModTime = std::filesystem::last_write_time(fragmentPath);
        bool changes = currentVertexModTime != lastVertexModTime || currentFragmentModTime != lastFragmentModTime;
        if (changes) {
            std::cout << "hot reloading shader" << std::endl;
            glDeleteProgram(ID);
            std::string vertexCode = loadShader(vertexPath);
            std::string fragmentCode = loadShader(fragmentPath);
            createShaderProgram(vertexCode, fragmentCode);
            lastVertexModTime = std::filesystem::last_write_time(vertexPath);
            lastFragmentModTime = std::filesystem::last_write_time(fragmentPath);
        }

        glUseProgram(ID);
    }

    void setBool(const std::string& name, bool value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }
    void setInt(const std::string& name, int value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    void setFloat(const std::string& name, float value) const {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
    void setVec3(const std::string& name, const glm::vec3& value) const {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    void setMat4(const std::string& name, const glm::mat4& mat) const {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

private:
    void createShaderProgram(const std::string& vertexCode, const std::string& fragmentCode) {
        // compile vertex shader
        unsigned int vertex;
        const char* vertexSource = vertexCode.c_str();
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vertexSource, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");

        // compile fragment shader
        unsigned int fragment;
        const char* fragmentSource = fragmentCode.c_str();
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fragmentSource, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");

        // create shader program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    std::string loadShader(const std::string& path) {
        std::ifstream file(path);
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    void checkCompileErrors(unsigned int shader, std::string type) {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM") {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << std::endl;
            }
        }
        else {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << std::endl;
            }
        }
    }
};

#endif 
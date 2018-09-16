#pragma once

#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <array>
#include <sstream>
#include <cassert>
#include <stdio.h>
#include <vector>

namespace TK {

void check(GLuint ShaderID) {
    GLint Result;
    GLint InfoLogLength;
    // Check Vertex Shader
    glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(ShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> ShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(ShaderID, InfoLogLength, NULL, &ShaderErrorMessage[0]);
        printf("%s\n", &ShaderErrorMessage[0]);
    } else {
        std::cout << "Shader Compile -- Success" << std::endl;
    }
}

template<std::size_t N = 8>
class ShadeLoader {
GLuint ProgramID{};
int pointer{};
std::array<GLint, N> shader_ids{};
public:
    template<class Shader>
    decltype(auto) add_shader(std::ifstream& input) noexcept {
        GLuint ShaderID = Shader::load();
        std::stringstream ss{};
        ss << input.rdbuf();
        std::string code = ss.str();
        const char* data = code.c_str();
        // Compile Vertex Shader
        glShaderSource(ShaderID, 1, &data, NULL);
        glCompileShader(ShaderID);
        check(ShaderID);
        shader_ids[pointer++] = ShaderID;
    };
    const auto compile() noexcept {
        ProgramID = glCreateProgram();
        int index = 0;
        for (auto shader_id = std::cbegin(shader_ids);
            shader_id != std::cend(shader_ids) && index < pointer; shader_id++) {
            glAttachShader(ProgramID, *shader_id);
            index ++;
        }
        glLinkProgram(ProgramID);
        return ProgramID;
    };
    ShadeLoader() = default;
    ~ShadeLoader() {
        for (auto shader_id = std::cbegin(shader_ids);
            shader_id != std::cend(shader_ids); shader_id++) {
            glDetachShader(ProgramID, *shader_id);
            glDeleteShader(*shader_id);
        }
    };
};

class VertexShader {
public:
    decltype(auto) static load() noexcept {
        return glCreateShader(GL_VERTEX_SHADER);
    };
};

class GeometryShader {
public:
    decltype(auto) static load() noexcept {
        return glCreateShader(GL_GEOMETRY_SHADER);
    };
};

class FragmentShader {
public:
    decltype(auto) static load() noexcept {
        return glCreateShader(GL_FRAGMENT_SHADER);
    };
};

}
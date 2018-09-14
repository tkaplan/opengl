#include <iostream>
#include <fstream>
#include <tuple>
#include <array>
#include <utility>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "ShadeLoader/ShadeLoader.hxx"

constexpr auto DEFAULT_WINDOW_HEIGHT = 512;
constexpr auto DEFAULT_WINDOW_WIDTH = 512;

constexpr auto static_display = [](auto programId, auto vertexBuffer){
    return [=](auto window){
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        // Lets just open a window
        glUseProgram(programId);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(GLfloat),nullptr);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDisableVertexAttribArray(0);
        glfwSwapBuffers(window);
    };
};

constexpr auto getProgramId = []() noexcept {
    TK::ShadeLoader shader{};

    // Vertex Shader
    std::ifstream vertex{"Shaders/vertex.glsl", std::ios::in};
    shader.template add_shader<TK::VertexShader>(vertex);
    vertex.close();
    // Fragment Shader
    std::ifstream fragment{"Shaders/fragment.glsl", std::ios::in};
    shader.template add_shader<TK::FragmentShader>(fragment);
    fragment.close();

    return shader.compile();
};

constexpr auto initGL = [](){
    glfwInit();
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    auto window = glfwCreateWindow( 1024, 768, "OpenGL Experiments", NULL, NULL);
    glfwMakeContextCurrent(window);
    // initialize necessary OpenGL extensions
    glewExperimental = GL_TRUE; 
    glewInit();
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    return std::make_tuple(getProgramId(), window);
};

class RenderVAO {
    const GLuint programId_{};
    const int vaos_{};
    const int vbos_{};
    GLuint vertexArrayId_{};
    GLuint vertexBuffer_{};
public:
    RenderVAO(const GLuint programId,
            const int vaos = 1,
            const int vbos = 1) : programId_{programId},
                vaos_{1}, vbos_{1} {
        glGenVertexArrays(vaos, &vertexArrayId_);
        glBindVertexArray(vertexArrayId_);
        glGenBuffers(vbos, &vertexBuffer_);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer_);
    }
    ~RenderVAO() {
        glDeleteBuffers(vbos_, &vertexBuffer_);
        glDeleteVertexArrays(vaos_, &vertexArrayId_);
        glDeleteProgram(programId_);
    }
    template<std::size_t N, class T>
    auto bind(const std::array<GLfloat, N> &g_vertex_buffer_data, const T& factory) {
        glBufferData(GL_ARRAY_BUFFER, g_vertex_buffer_data.size() * sizeof(GLfloat), g_vertex_buffer_data.data(), GL_STATIC_DRAW);
        return factory(programId_, vertexBuffer_);
    }
};

template<class TA, class TB>
void glLoop(TA&& display, TB& window) {
    do{std::forward<TA>(display)(window);glfwPollEvents();}while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS and
        glfwWindowShouldClose(window) == 0);
};

int main(int argcp, char ** argv) {
    static const std::array<GLfloat, 9> g_vertex_buffer_data {
        -1.0f, -1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
         0.0f,  1.0f, 0.0f
    };

    const auto[programId, window] = initGL();
    RenderVAO render{programId};
    glLoop(render.bind(g_vertex_buffer_data, static_display), window);
    glfwTerminate();
}
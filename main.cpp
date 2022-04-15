#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <tuple>

#include "ShaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

auto drawCircle(GLfloat vertices[], GLuint indices[], size_t planet_idx, GLfloat radius, GLfloat x, GLfloat y, std::tuple<GLfloat, GLfloat, GLfloat> color, size_t n) -> void;
auto drawEmptyCircle(GLfloat vertices[], GLuint indices[], size_t indices_idx, GLfloat radius, GLfloat x, GLfloat y, std::tuple<GLfloat, GLfloat, GLfloat> color, size_t n) -> void;
auto drawSolarSystem(GLfloat vertices[], GLuint indices[], size_t n, size_t iter) -> void;

using std::cout, std::endl;


int main(){

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(1200, 900, "Solar System", nullptr, nullptr);


    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    gladLoadGL();

    constexpr size_t n = 60;
    constexpr size_t numberOfPlanets = 9;
    GLfloat vertices[(2 * numberOfPlanets - 1) * ((n + 1) * 5)] {};
    GLuint indices[(2 * numberOfPlanets  - 1)* n * 3] {};

    Shader shaderProgram("default.vert", "default.frag");

    VAO VAO1;
    VAO1.Bind();

    drawSolarSystem(vertices, indices, n, 0);
    VBO VBO1(vertices, sizeof(vertices));
    EBO EBO1(indices, sizeof(indices));

    VAO1.LinkVBO(VBO1, 0, 1);

    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();


    glClearColor(0.58f, 0.0f, 0.83f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    size_t tmp = 0;

    glfwSwapBuffers(window);
    while(!glfwWindowShouldClose(window)){
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shaderProgram.Activate();
        VAO1.Bind();
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        for(int i = 0; i < numberOfPlanets - 1; ++i){
            glDrawElements(GL_LINE_STRIP, n, GL_UNSIGNED_INT, (void *) ((n * (3 * numberOfPlanets + i))* sizeof(GLuint)));
        }
        glDrawElements(GL_TRIANGLES, 3 * n * numberOfPlanets, GL_UNSIGNED_INT, nullptr);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


        drawSolarSystem(vertices, indices, n, tmp);
        VBO1.Update(vertices, sizeof(vertices));
        VAO1.LinkVBO(VBO1, 0, 1);
        VAO1.Unbind();
        glfwSwapBuffers(window);
        glfwPollEvents();
        ++tmp;
    }

    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    shaderProgram.Delete();


    glfwDestroyWindow(window);
    glfwTerminate();
}

auto drawCircle(GLfloat vertices[], GLuint indices[], size_t planet_idx, GLfloat radius, GLfloat x, GLfloat y, std::tuple<GLfloat, GLfloat, GLfloat> color, size_t n) -> void{
    vertices[0] = x; vertices[1] = y; // center position
    vertices[2] = std::get<0>(color); vertices[3] = std::get<1>(color); vertices[4] = std::get<2>(color);
    double step = 360. / n;
    constexpr double to_radian = 0.0174532925;
    for(int i = 0, j = 5; j < (n + 1) * 5; i += step, j += 5){
        auto cos_ = static_cast<GLfloat>(cos(i * to_radian));
        auto sin_ = static_cast<GLfloat>(sin(i * to_radian));
        vertices[j] = x + radius * cos_; vertices[j + 1] = y + radius * sin_; // position (x, y)
        vertices[j + 2] = std::get<0>(color); vertices[j + 3] = std::get<1>(color); vertices[j + 4] = std::get<2>(color); // color (r, g, b)
    }
    int indices_idx = planet_idx * (n + 1);
    for(int i = 0, j = indices_idx + 1; i < (n - 1) * 3; i += 3, ++j){
        indices[i] = indices_idx;
        indices[i + 1] = j;
        indices[i + 2] = j + 1;
    }
    indices[3 * (n - 1)] = indices_idx;
    indices[3 * (n - 1) + 1] = indices[1]; 
    indices[3 * (n - 1) + 2] = indices[(n - 1) * 3 - 1];
}

auto drawEmptyCircle(GLfloat vertices[], GLuint indices[], size_t indices_idx, GLfloat radius, GLfloat x, GLfloat y, std::tuple<GLfloat, GLfloat, GLfloat> color, size_t n) -> void{
    double step = 360. / n;
    constexpr double to_radian = 0.0174532925;
    for(int i = 0, j = 0; j < (n + 1) * 5; i += step, j += 5){
        auto cos_ = static_cast<GLfloat>(cos(i * to_radian));
        auto sin_ = static_cast<GLfloat>(sin(i * to_radian));
        vertices[j] = 3 * (x + radius * cos_); vertices[j + 1] = 2 * (y + radius * sin_); // position (x, y)
        vertices[j + 2] = std::get<0>(color); vertices[j + 3] = std::get<1>(color); vertices[j + 4] = std::get<2>(color); // color (r, g, b)
    }
    for(int i = 0, j = indices_idx; i < n; ++i, ++j){
        indices[i] = j;
    }
    indices[n - 1] = indices_idx;
}

auto drawSolarSystem(GLfloat vertices[], GLuint indices[], size_t n, size_t iter) ->void{
    constexpr GLfloat planetRadius[9] {696.34 * 1e3, 2439.7, 6051.8, 6371,
                                     3389.5, 69911, 58232, 25362, 24622};
    constexpr GLfloat scaledRadius[9] {0.05,
                                        2.f * planetRadius[1] / planetRadius[0],
                                        2.f * planetRadius[2] / planetRadius[0],
                                        2.f * planetRadius[3] / planetRadius[0],
                                        2.f * planetRadius[4] / planetRadius[0],
                                        0.4f * planetRadius[5] / planetRadius[0],
                                        0.4f * planetRadius[6] / planetRadius[0],
                                        0.4f * planetRadius[7] / planetRadius[0],
                                        0.4f * planetRadius[8] / planetRadius[0]};

    constexpr GLfloat positions[9] {0, 0.05, 0.09, 0.13, 0.17, 0.21, 0.25, 0.29, 0.33};
    constexpr std::tuple<GLfloat, GLfloat, GLfloat> planetColor[9] {std::make_tuple(0.956f, 1.f, 0.043f), std::make_tuple(0.592f, 0.592f, 0.592f),
                                                                     std::make_tuple(0.596f, 0.627f, 0.f), std::make_tuple(0.f, 0.282f, 0.772f),
                                                                     std::make_tuple(0.807f, 0.035f, 0.01f), std::make_tuple(0.87f, 0.419f, 0.f),
                                                                     std::make_tuple(0.76f, 0.533f, 0.317f), std::make_tuple(0.298f, 0.854f, 0.796f),
                                                                     std::make_tuple(0.105f, 0.f, 1.f),
                                                                     };
    constexpr double planetSpeed[9] {0, 1, 0.345, 0.241, 0.128, 0.02, 0.0083, 0.0028, 0.00146};
    constexpr double speed_scale = 0.3;

    // ************************************************************ DRAWING PLANETS (AND SUN) **************************************************************
    for(int i = 0; i < 9; ++i){
        drawCircle(vertices + i * (n + 1) * 5, indices + i * n * 3, i, scaledRadius[i],
                   3 * positions[i] * static_cast<GLfloat>(cos(planetSpeed[i] * iter * speed_scale)), 2 * positions[i] * static_cast<GLfloat>(sin(planetSpeed[i] * iter * speed_scale)),
                   planetColor[i], n);
    }
    // ************************************************************ DRAWING ORBITS **************************************************************
    size_t orbits_vertices_offset = 9 * (n + 1) * 5;
    size_t orbits_indices_offset = 9 * n * 3;
    size_t orbits_indices_idx = 9 * (n + 1);
    for(int i = 0; i < 8; ++i) {
        drawEmptyCircle(vertices + orbits_vertices_offset + i * (n + 1) * 5, indices + orbits_indices_offset + i * n,
                        orbits_indices_idx + i * (n + 1),
                        positions[i + 1],
                        0, 0,
                        std::make_tuple(1.f, 1.f, 1.f),
                        n);
    }
}
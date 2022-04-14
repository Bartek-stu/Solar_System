#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

#include "ShaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

auto drawCircle(GLfloat vertices[], GLuint indices[], size_t planet_idx, GLfloat radius, GLfloat x, GLfloat y, std::tuple<GLfloat, GLfloat, GLfloat> color, size_t n) -> void;
auto drawSolarSystem(GLfloat vertices[], GLuint indices[], size_t n, size_t iter) -> void;

using std::cout, std::endl;


int main(){

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(1200, 800, "Opengl-window", nullptr, nullptr);


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
//        glUseProgram(shaderProgram);
        shaderProgram.Activate();
        VAO1.Bind();
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//        glMatrixMode(GL_PROJECTION);
//        glLoadIdentity();
        glDrawElements(GL_TRIANGLES, 3 * n * numberOfPlanets, GL_UNSIGNED_INT, nullptr);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLES, 3 * n * (numberOfPlanets - 1), GL_UNSIGNED_INT, (void*)(3 * n * numberOfPlanets * sizeof(GLuint)));


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
    vertices[2] = 1.f; vertices[3] = 1.f; vertices[4] = 1.f;
    double step = 360 / n;
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

    constexpr GLfloat positions[9] {0, 0.1, 0.225, 0.35, 0.475, 0.6, 0.725, 0.85, 0.9};
    constexpr double speed_scale = 0.3;
    auto sin_ = static_cast<GLfloat>(sin(iter * speed_scale));
    auto cos_ = static_cast<GLfloat>(cos(iter * speed_scale));


    // ************************************************************ DRAWING PLANETS (AND SUN) **************************************************************
    drawCircle(vertices, indices, 0,
               scaledRadius[0],
               positions[0] * cos_, positions[0] * sin_,
               std::make_tuple(1.f, 1.f, 1.f),
               n);
    drawCircle(vertices + (n + 1) * 5, indices + (n * 3), 1,
               scaledRadius[1],
               positions[1] * static_cast<GLfloat>(cos(iter * speed_scale)), positions[1] * static_cast<GLfloat>(sin(iter * speed_scale)),
               std::make_tuple(1.f, 1.f, 1.f),
               n);
    drawCircle(vertices + 2 * (n + 1) * 5, indices + 2 * (n * 3), 2,
               scaledRadius[2],
               positions[2] * static_cast<GLfloat>(cos(0.345 * iter * speed_scale)), positions[2] * static_cast<GLfloat>(sin(0.345 * iter * speed_scale)),
               std::make_tuple(1.f, 1.f, 1.f),
               n);
    drawCircle(vertices + 3 * (n + 1) * 5, indices + 3 * (n * 3),3,
               scaledRadius[3],
               positions[3] * static_cast<GLfloat>(cos(0.241 * iter * speed_scale)), positions[3] * static_cast<GLfloat>(sin(0.241 * iter * speed_scale)),
               std::make_tuple(1.f, 1.f, 1.f),
               n);
    drawCircle(vertices + 4 * (n + 1) * 5, indices + 4 * (n * 3),4,
               scaledRadius[4],
               positions[4] * static_cast<GLfloat>(cos(0.128 * iter * speed_scale)), positions[4] * static_cast<GLfloat>(sin(0.128 * iter * speed_scale)),
               std::make_tuple(1.f, 1.f, 1.f),
               n);
    drawCircle(vertices + 5 * (n + 1) * 5, indices + 5 * (n * 3),5,
               scaledRadius[5],
               positions[5] * static_cast<GLfloat>(cos(0.02 * iter * speed_scale)), positions[5] * static_cast<GLfloat>(sin(0.02 * iter * speed_scale)),
               std::make_tuple(1.f, 1.f, 1.f),
               n);
    drawCircle(vertices + 6 * (n + 1) * 5, indices + 6 * (n * 3),6,
               scaledRadius[6],
               positions[6] * static_cast<GLfloat>(cos(0.0083 * iter * speed_scale)), positions[6] * static_cast<GLfloat>(sin(0.0083 * iter * speed_scale)),
               std::make_tuple(1.f, 1.f, 1.f),
               n);
    drawCircle(vertices + 7 * (n + 1) * 5, indices + 7 * (n * 3),7,
               scaledRadius[7],
               positions[7] * static_cast<GLfloat>(cos(0.0028 * iter * speed_scale)), positions[7] * static_cast<GLfloat>(sin(0.0028 * iter * speed_scale)),
               std::make_tuple(1.f, 1.f, 1.f),
               n);
    drawCircle(vertices + 8 * (n + 1) * 5, indices + 8 * (n * 3),8,
               scaledRadius[8],
               positions[8] * static_cast<GLfloat>(cos(0.00146 * iter * speed_scale)), positions[8] * static_cast<GLfloat>(sin(0.00146 * iter * speed_scale)),
               std::make_tuple(1.f, 1.f, 1.f),
               n);

    // ************************************************************ DRAWING ORBITS **************************************************************
    drawCircle(vertices + 9 * (n + 1) * 5, indices + 9 * (n * 3),9,
               positions[1],
               0, 0,
               std::make_tuple(1.f, 1.f, 1.f),
               n);
    drawCircle(vertices + 10 * (n + 1) * 5, indices + 10 * (n * 3),10,
               positions[2],
               0, 0,
               std::make_tuple(1.f, 1.f, 1.f),
               n);
    drawCircle(vertices + 11 * (n + 1) * 5, indices + 11 * (n * 3),11,
               positions[3],
               0, 0,
               std::make_tuple(1.f, 1.f, 1.f),
               n);
    drawCircle(vertices + 12 * (n + 1) * 5, indices + 12 * (n * 3),12,
               positions[4],
               0, 0,
               std::make_tuple(1.f, 1.f, 1.f),
               n);
    drawCircle(vertices + 13 * (n + 1) * 5, indices + 13 * (n * 3),13,
               positions[5],
               0, 0,
               std::make_tuple(1.f, 1.f, 1.f),
               n);
    drawCircle(vertices + 14 * (n + 1) * 5, indices + 14 * (n * 3),14,
               positions[6],
               0, 0,
               std::make_tuple(1.f, 1.f, 1.f),
               n);
    drawCircle(vertices + 15 * (n + 1) * 5, indices + 15 * (n * 3),15,
               positions[7],
               0, 0,
               std::make_tuple(1.f, 1.f, 1.f),
               n);
    drawCircle(vertices + 16 * (n + 1) * 5, indices + 16 * (n * 3),16,
               positions[8],
               0, 0,
               std::make_tuple(1.f, 1.f, 1.f),
               n);
}
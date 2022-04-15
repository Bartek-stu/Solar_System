#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <tuple>

#include "ShaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Constants.h"

auto drawCircle(GLfloat vertices[], GLuint indices[], size_t planet_idx, GLfloat radius, GLfloat x, GLfloat y, std::tuple<GLfloat, GLfloat, GLfloat> color) -> void;
auto drawEmptyCircle(GLfloat vertices[], GLuint indices[], size_t indices_idx, GLfloat radius, GLfloat x, GLfloat y, std::tuple<GLfloat, GLfloat, GLfloat> color,
                     GLfloat a, GLfloat b) -> void;
auto drawSolarSystem(GLfloat vertices[], GLuint indices[], size_t iter) -> void;

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


    GLfloat vertices[verticesSize] {};
    GLuint indices[indicesSize] {};

    Shader shaderProgram("default.vert", "default.frag");

    VAO VAO1;
    VAO1.Bind();

    drawSolarSystem(vertices, indices, 0);
    VBO VBO1(vertices, sizeof(vertices));
    EBO EBO1(indices, sizeof(indices));

    VAO1.LinkVBO(VBO1, 0, 1);

    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    size_t tmp = 0;

    glfwSwapBuffers(window);
    while(!glfwWindowShouldClose(window)){
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shaderProgram.Activate();
        VAO1.Bind();

        for(int i = 0; i < numberOfPlanets - 1 + numberOfRings; ++i){
            glDrawElements(GL_LINE_STRIP, n, GL_UNSIGNED_INT, (void *) ((n * (3 * numberOfPlanets + i))* sizeof(GLuint)));
        }
        glDrawElements(GL_TRIANGLES, 3 * n * numberOfPlanets, GL_UNSIGNED_INT, nullptr);

        drawSolarSystem(vertices, indices, tmp);
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

auto drawCircle(GLfloat vertices[], GLuint indices[], size_t planet_idx, GLfloat radius, GLfloat x, GLfloat y, std::tuple<GLfloat, GLfloat, GLfloat> color) -> void{
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

auto drawEmptyCircle(GLfloat vertices[], GLuint indices[], size_t indices_idx, GLfloat radius, GLfloat x, GLfloat y, std::tuple<GLfloat, GLfloat, GLfloat> color,
                     GLfloat a, GLfloat b) -> void{
    double step = 360. / n;
    constexpr double to_radian = 0.0174532925;
    for(int i = 0, j = 0; j < (n + 1) * 5; i += step, j += 5){
        auto cos_ = static_cast<GLfloat>(cos(i * to_radian));
        auto sin_ = static_cast<GLfloat>(sin(i * to_radian));
        vertices[j] = a * (x + radius * cos_); vertices[j + 1] = b * (y + radius * sin_); // position (x, y)
        vertices[j + 2] = std::get<0>(color); vertices[j + 3] = std::get<1>(color); vertices[j + 4] = std::get<2>(color); // color (r, g, b)
    }
    for(int i = 0, j = indices_idx; i < n; ++i, ++j){
        indices[i] = j;
    }
    indices[n - 1] = indices_idx;
}

auto drawSolarSystem(GLfloat vertices[], GLuint indices[], size_t iter) ->void{
    // ************************************************************ DRAWING PLANETS (AND SUN) **************************************************************
    for(int i = 0; i < numberOfPlanets; ++i){
        drawCircle(vertices + i * (n + 1) * 5, indices + i * n * 3, i, scaledRadius[i],
                   a_ellipse * positions[i] * static_cast<GLfloat>(cos(planetSpeed[i] * iter * speed_scale)),
                   b_ellipse * positions[i] * static_cast<GLfloat>(sin(planetSpeed[i] * iter * speed_scale)),
                   planetColor[i]);
    }
    // ************************************************************ DRAWING ORBITS **************************************************************
    size_t orbits_vertices_offset = 9 * (n + 1) * 5;
    size_t orbits_indices_offset = 9 * n * 3;
    size_t orbits_indices_idx = 9 * (n + 1);
    for(int i = 0; i < numberOfPlanets - 1; ++i) {
        drawEmptyCircle(vertices + orbits_vertices_offset + i * (n + 1) * 5, indices + orbits_indices_offset + i * n,
                        orbits_indices_idx + i * (n + 1),
                        positions[i + 1],
                        0, 0,
                        std::make_tuple(1.f, 1.f, 1.f),
                        a_ellipse, b_ellipse);
    }

    // saturn's rings
    auto tmp = 0.01f;
    for(int i = numberOfPlanets - 1; i < numberOfPlanets + 2; ++i){
        drawEmptyCircle(vertices + orbits_vertices_offset + i * (n + 1) * 5, indices + orbits_indices_offset + i * n,
                        orbits_indices_idx + i * (n + 1),
                        scaledRadius[6] + tmp,
                        a_ellipse * positions[6] * static_cast<GLfloat>(cos(planetSpeed[6] * iter * speed_scale)),
                        b_ellipse * positions[6] * static_cast<GLfloat>(sin(planetSpeed[6] * iter * speed_scale)),
                        std::make_tuple(1.f, 1.f, 1.f),
                        1, 1
                        );
        tmp += 0.005f;
    }
    // uran's ring
    drawEmptyCircle(vertices + orbits_vertices_offset + (numberOfPlanets + 2) * (n + 1) * 5,
                    indices + orbits_indices_offset + (numberOfPlanets + 2) * n,
                    orbits_indices_idx + (numberOfPlanets + 2) * (n + 1),
                    scaledRadius[7] + 0.01f,
                    a_ellipse * positions[7] * static_cast<GLfloat>(cos(planetSpeed[7] * iter * speed_scale)),
                    b_ellipse * positions[7] * static_cast<GLfloat>(sin(planetSpeed[7] * iter * speed_scale)),
                    std::make_tuple(1.f, 1.f, 1.f),
                    1, 1);
    // neptune's ring
    drawEmptyCircle(vertices + orbits_vertices_offset + (numberOfPlanets + 3) * (n + 1) * 5,
                    indices + orbits_indices_offset + (numberOfPlanets + 3) * n,
                    orbits_indices_idx + (numberOfPlanets + 3) * (n + 1),
                    scaledRadius[8] + 0.01f,
                    a_ellipse * positions[8] * static_cast<GLfloat>(cos(planetSpeed[8] * iter * speed_scale)),
                    b_ellipse * positions[8] * static_cast<GLfloat>(sin(planetSpeed[8] * iter * speed_scale)),
                    std::make_tuple(1.f, 1.f, 1.f),
                    1, 1);

}
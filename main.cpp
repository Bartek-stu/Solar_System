#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

#include "ShaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

auto drawCircle(GLfloat vertices[], GLuint indices[], size_t planet_idx, GLfloat radius, GLfloat x, GLfloat y, size_t n) -> void;
auto drawSolarSystem(GLfloat vertices[], GLuint indices[], size_t n) -> void;

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
    constexpr size_t numberOfPlanets = 7;
    GLfloat vertices[numberOfPlanets * ((n + 1) * 5)] {};
    GLuint indices[numberOfPlanets * n * 3] {};

    drawSolarSystem(vertices, indices, n);


//    for(int i = 0; i < 2 * (n + 1) * 5; ++i){
//        cout << vertices[i] << "\t";
//        if(!((i + 1) % 5)) cout << endl;
//    }
//    for(int i = 0; i < 2 * n * 3; ++i){
//        cout << indices[i] << "\t";
//        if(!((i + 1) % 3)) cout << endl;
//    }
    Shader shaderProgram("default.vert", "default.frag");

    VAO VAO1;
    VAO1.Bind();

    VBO VBO1(vertices, sizeof(vertices));
    EBO EBO1(indices, sizeof(indices));

    VAO1.LinkVBO(VBO1, 0, 1);

    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();


    glClearColor(0.58f, 0.0f, 0.83f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(window);
    while(!glfwWindowShouldClose(window)){
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
//        glUseProgram(shaderProgram);
        shaderProgram.Activate();
        VAO1.Bind();
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawElements(GL_TRIANGLES, 3 * n * numberOfPlanets, GL_UNSIGNED_INT, nullptr);

//        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(3 * sizeof(GLuint)));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    shaderProgram.Delete();


    glfwDestroyWindow(window);
    glfwTerminate();
}

auto drawCircle(GLfloat vertices[], GLuint indices[], size_t planet_idx, GLfloat radius, GLfloat x, GLfloat y, size_t n) -> void{
    vertices[0] = x; vertices[1] = y; // center position
    vertices[2] = 1.f; vertices[3] = 1.f; vertices[4] = 1.f;
    double step = 360 / n;
    constexpr double to_radian = 0.0174532925;
    for(int i = 0, j = 5; j < (n + 1) * 5; i += step, j += 5){
        auto cos_ = static_cast<GLfloat>(cos(i * to_radian));
        auto sin_ = static_cast<GLfloat>(sin(i * to_radian));
        vertices[j] = x + radius * cos_; vertices[j + 1] = y + radius * sin_; // position (x, y)
        vertices[j + 2] = 1.f; vertices[j + 3] = 1.f; vertices[j + 4] = 1.f; // color (r, g, b)
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

auto drawSolarSystem(GLfloat vertices[], GLuint indices[], size_t n) ->void{
    drawCircle(vertices, indices, 0, 0.1f, 0, 0, n);
    drawCircle(vertices + (n + 1) * 5, indices + (n * 3), 1, 0.05f, 0.3, 0, n);
    drawCircle(vertices + 2 * (n + 1) * 5, indices + 2 * (n * 3), 2, 0.02f, 0.5, 0, n);
    drawCircle(vertices + 3 * (n + 1) * 5, indices + 3 * (n * 3), 3, 0.08f, 0.7, 0, n);
}
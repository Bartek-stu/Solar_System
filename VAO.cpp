#include "VAO.h"

VAO::VAO(){
    glGenVertexArrays(1, &ID);
}

void VAO::LinkVBO(VBO &VBO, GLuint positionLocation, GLuint colorLocation) {
    VBO.Bind();
    glEnableVertexAttribArray(positionLocation);
    glVertexAttribPointer(positionLocation, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), 0);

    glEnableVertexAttribArray(colorLocation);
    glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
    VBO.Unbind();
}

void VAO::Bind(){
    glBindVertexArray(ID);
}

void VAO::Unbind(){
    glBindVertexArray(0);
}

void VAO::Delete(){
    glDeleteVertexArrays(1, &ID);

}
